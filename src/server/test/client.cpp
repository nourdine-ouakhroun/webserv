

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// #define SERVER_IP "127.0.0.1" // Update with your server IP
// #define SERVER_PORT 80       // Update with your server port

int sendRequest(int port) {
    int sockfd = 0, valread;
    struct sockaddr_in serv_addr;
    std::string message = "Hello from the client";
    char response[1024] = {0};

    // Creating socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Convert IPv4 and IPv6 addresses from text to binary form
    // if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
    //     printf("\nInvalid address/ Address not supported \n");
    //     return -1;
    // }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Send data to the server
    write(sockfd, "hello\r\n\r\n", strlen("hello\r\n\r\n"));
    printf("Message sent to the server\n");

    // Read the server response
    valread = read(sockfd, response, 1024);
    printf("%s\n", response);

    return 0;
}

int main()
{
        sendRequest(80);

    // int i = -1;
    // while (++i < 100)
    // {
    //     sendRequest(8080);
    //     std::cout << "\nRequest : " << i << std::endl;
    //     // usleep(1000);
    // }
}
