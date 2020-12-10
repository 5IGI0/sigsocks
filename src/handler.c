#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "config.h"
#include "protocol.h"
#include "utils.h"
#include "connect.h"

#include "libs/logger/log.h"

#define CONNECTION_TIMEOUT 10
#define BUFFER_SIZE 2500

int setNonBlocking(int __fd) {
    long arg; // socket args

    // Set non-blocking 
    if((arg = fcntl(__fd, F_GETFL, NULL)) < 0)
        return -1;

    arg |= O_NONBLOCK; 
    if(fcntl(__fd, F_SETFL, arg) < 0)
        return -1;

    return 0;
}

ssize_t nnread(int __fd, void *buff, size_t size) {
    ssize_t tmp = read(__fd, buff, size);
    if (tmp > -1)
        return tmp;
    else if (errno == EAGAIN) {
        errno = 0;
        return -2;
    }
    return -1;
}

bool connectionAuth(int sock, config_t config) {

    (void) config; // TODO: implement login-password method
    uint8_t version = 0;
    uint8_t nmethod = 0;
    bool success = false;
    uint8_t method = 0;
    size_t i = 0; // iterator

    recv(sock, &version, 1, MSG_NOSIGNAL);
    logVerbose("client version: %d\n", version);
    recv(sock, &nmethod, 1, MSG_NOSIGNAL);

    for (i = 0; i < nmethod; i++) {
        recv(sock, &method, 1, MSG_NOSIGNAL);

        if (method == 0)
            success = true;
    }
    
    if (success)
        send(sock, "\x05\x00", 2, MSG_NOSIGNAL);
    else
        send(sock, "\x05\xFF", 2, MSG_NOSIGNAL);

    return success;
}

void sendReply(int sock, sockRequest_t request, uint8_t status) {
    send(sock, "\x05", 1, MSG_NOSIGNAL);
    send(sock, &status, 1, MSG_NOSIGNAL);
    send(sock, "\x00\x01", 2, MSG_NOSIGNAL);
    send(sock, &request.addr, 4, MSG_NOSIGNAL);
    send(sock, &request.port, 2, MSG_NOSIGNAL);
}

bool connectionReadRequest(int sock, sockRequest_t *request) {

    recv(sock, &request->version, 1, MSG_NOSIGNAL);
    recv(sock, &request->command, 1, MSG_NOSIGNAL);
    
    if (request->command != CMD_CONNECT) { // if insupported
        sendReply(sock, *request, REP_CMD_NOT_SUPPORTED);
        return false;
    }
    
    recv(sock, &request->reserved, 1, MSG_NOSIGNAL);
    recv(sock, &request->addrType, 1, MSG_NOSIGNAL);

    if (request->addrType != ATYPE_IPV4) {// if insupported
        sendReply(sock, *request, RAP_ADDR_TYPE_NOT_SUPPORTED);
        return false;
    }

    recv(sock, &request->addr, 4, MSG_NOSIGNAL);
    recv(sock, &request->port, 2, MSG_NOSIGNAL);

    return true;
}

struct sockaddr *connectionRequestToSockAddr(sockRequest_t request) {

    static struct sockaddr_in sock = {0}; // target

    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = request.addr;
    sock.sin_port = request.port;

    return (struct sockaddr *)&sock;
}

void connectionHandler(int sock, config_t config, struct sockaddr *sin) {
    
    sockRequest_t request = {0};
    int tsock = 0; // target socket
    struct sockaddr *target = NULL;
    ssize_t res = 0;
    uint8_t buffer[BUFFER_SIZE] = "";

    if (connectionAuth(sock, config) == false)
        return;
    
    if (connectionReadRequest(sock, &request) == false)
        return;

    target = connectionRequestToSockAddr(request);

    logPrintf("%s", get_ip_str(sin));
    logPrintf(" <-> %s:%hu\n", get_ip_str(target), ntohs(request.port));

    tsock = socket(target->sa_family, SOCK_STREAM, 0);

    if (tsock < 0) {
        sendReply(sock, request, REP_FAILURE);
        return;
    }

    if(connectTimeout(tsock, target, sizeof(struct sockaddr_in), CONNECTION_TIMEOUT) < 0) {
        sendReply(sock, request, REP_HOST_UNREACHABLE);
        close(tsock);
        return;
    }

    if(setNonBlocking(sock) < 0 || setNonBlocking(tsock) < 0) {
        sendReply(sock, request, REP_FAILURE);
        close(tsock);
        return;
    }

    sendReply(sock, request, REP_SUCCEEDED);

    while (true) {
        // TODO: use select
        usleep(1000);
        
        if((res = nnread(sock, buffer, BUFFER_SIZE)) > 0)
            write(tsock, buffer, res);
        else if (res == -1) {
            close(tsock);
            return;
        }
        
        if((res = nnread(tsock, buffer, BUFFER_SIZE)) > 0)
            write(sock, buffer, res);
        else if (res == -1) {
            close(tsock);
            return;
        }

    }
}