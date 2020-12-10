#include <stdlib.h>
#include <arpa/inet.h>

// "fork" of https://gist.github.com/jkomyno/45bee6e79451453c7bbdc22d033a282e
char *get_ip_str(const struct sockaddr *sa) {
    static char buffer[INET6_ADDRSTRLEN+1] = "";

    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), buffer, INET6_ADDRSTRLEN+1);
            break;
        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),buffer, INET6_ADDRSTRLEN+1);
            break;
        default:
            return NULL;
    }

    return buffer;
}