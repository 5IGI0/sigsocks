#include <unistd.h>
#include <arpa/inet.h>

#include "config.h"

int serverInit(config_t config) {

    // for noobs: AF_INET = IPv4
    // for noobs: SOCK_STREAM = TCP/IP
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};

    if (sock < 0)
        return -__LINE__;
    
    // setup listening infos
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // for noobs: listen on all IPs
    addr.sin_port = htons(config.listening_port);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
        close(sock);
        return -__LINE__;
    }

    if (listen(sock, 5)) {
        close(sock);
        return -__LINE__;
    }

    return sock;
}