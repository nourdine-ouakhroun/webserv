#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<unistd.h>
#include<iostream>
#define MYPORT 8080 
#define BACKLOG 10
int main(int argc, char *argv[])
{
	struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct sockaddr_in servaddr;
    int sockfd, new_fd;



    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(MYPORT);
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
   	listen(sockfd, BACKLOG);
	socklen_t address_len = sizeof(servaddr);
	while(1)
	{

		char str[2024];
   		addr_size = sizeof their_addr;
    	new_fd = accept(sockfd, (struct sockaddr *)&servaddr, &address_len);
		read(new_fd, str,2024);
		std::cout << str << std::endl;
		//char *str1 = "HTTP/1.0 200 OK\nContent-Type: text/html\n\nHELLO";
		//write(new_fd, str1, sizeof(str1));

		std::string server_req_h;
		std::string req_body = "<h1>HELLO WORLD </h1>";
        server_req_h.append( "HTTP/1.1 200 OK\n");
        server_req_h.append( "Content-Type: text/html\n");
        server_req_h.append( "Content-Length: ").append(std::to_string(req_body.length())).append("\n");
        server_req_h.append( "\n");
        server_req_h.append(req_body).append("\n");

		write(new_fd, server_req_h.c_str(), server_req_h.length());
		close(new_fd);
	}
}
