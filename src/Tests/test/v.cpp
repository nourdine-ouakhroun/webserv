
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    struct addrinfo hints, *res;
    int sockfd, status;
    char port[6];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    sprintf(port, "%d", 0); // Port 0 for random assignment

    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        return 2;
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return 2;
    }

    // Get the port that was assigned
    struct sockaddr_in addr_in;
    socklen_t len = sizeof(addr_in);
    getsockname(sockfd, (struct sockaddr *)&addr_in, &len);
    printf("Random free port: %d\n", ntohs(addr_in.sin_port));

    freeaddrinfo(res);
    // close(sockfd);

    return 0;
}