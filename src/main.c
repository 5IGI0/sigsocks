#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#include "config.h"
#include "serverInit.h"
#include "utils.h"
#include "handler.h"

#include "libs/logger/log.h"

static int _is_stopped = false;
int _ssock = 0; // server socket

void sighandler(int signum) {
    (void)signum;
    _is_stopped = true;
    close(_ssock);
}

int main(int argc, char const **argv) {
    int csock = 0; // client socket
    struct sockaddr_in caddr; // client addr
    int addrlen = sizeof(caddr);
    config_t config = {0};

    signal(SIGINT, sighandler);
    signal(SIGKILL, sighandler);
    
    if(readConfig(&config, argc, argv))
        return -__LINE__;

    if ((_ssock = serverInit(config)) < 0) {
        perror("server cannot be initiated");
        return -__LINE__;
    }

    logPrintf("listening on %hu...\n", config.listening_port);

    while (_is_stopped == false) {
        csock = accept(_ssock, (struct sockaddr *)&caddr, (socklen_t *)&addrlen);

        if (csock < 0) {
            if (_is_stopped)
                puts("stopping...");
            else
                perror("accept");
            break;
        }

        logVerbose("new connection from %s\n", get_ip_str((struct sockaddr *)&caddr));

        if(fork() == 0) {
            close(_ssock);
            connectionHandler(csock, config, (struct sockaddr *)&caddr);
            close(csock);
            return 0;
        }
        
        close(csock);
    }

    close(_ssock);
    
    return 0;
}
