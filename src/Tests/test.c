#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CONNECTIONS 5

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CONNECTIONS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client, &client_len)) == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    // Get the client's IP address
    inet_ntop(AF_INET, &(client.sin_addr), client_ip, INET_ADDRSTRLEN);

    printf("Connection accepted from %s:%d\n", client_ip, ntohs(client.sin_port));

    // Rest of your code...

    // Close the sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
