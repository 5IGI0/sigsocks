#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

int connectTimeout(int __fd, void *__addr, socklen_t __len, uint16_t timeout) {

    struct timeval tv; // timeout
    int res; // connection response
    long arg; // socket args
    fd_set myset;
    int valopt;
    socklen_t lon;

    tv.tv_usec = 0;
    tv.tv_sec  = timeout;

    // Set non-blocking 
    if((arg = fcntl(__fd, F_GETFL, NULL)) < 0)
        return -1;

    arg |= O_NONBLOCK; 
    if(fcntl(__fd, F_SETFL, arg) < 0)
        return -1;

    // Trying to connect with timeout 
    res = connect(__fd, __addr, __len); 
    
    if (res < 0) {
        if (errno == EINPROGRESS) {
            FD_ZERO(&myset); 
            FD_SET(__fd, &myset); 
            res = select(__fd+1, NULL, &myset, NULL, &tv); 
            if (res < 0 && errno != EINTR)
                return -1;
            else if (res > 0) {

                lon = sizeof(int);

                if (getsockopt(__fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0)
                    return -1;
                
                if (valopt) { 
                    errno = valopt;
                    return -1;
                } 
            } else
                return -2;
        }  else
            return -1;
    } 

    // Set to blocking mode again... 
    if((arg = fcntl(__fd, F_GETFL, NULL)) < 0)
        return -1;
    
    arg &= (~O_NONBLOCK); 
    if(fcntl(__fd, F_SETFL, arg) < 0) 
        return -1;

    return 0;
}