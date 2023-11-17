#include<sstream>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include<vector>
#include <sys/poll.h>
#include <sys/select.h>
int main()
{
	int socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if(socketfd < 0)
	{
		std::cout << "error : socket conection not open ." << std::endl;
		exit(1);
	}
    int option = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	struct sockaddr_in addressSocketStruct;
	bzero(&addressSocketStruct, sizeof(addressSocketStruct));
	addressSocketStruct.sin_family = PF_INET;
	addressSocketStruct.sin_port = htons(8099);
	addressSocketStruct.sin_addr.s_addr = INADDR_ANY;
	int bindr =  bind(socketfd,	(struct sockaddr *)&addressSocketStruct, sizeof(addressSocketStruct));
	if(bindr < 0)
	{
		std::cout << "error : the conectin not binded ." << std::endl;
		exit(0);
	}
	int lestenr = listen(socketfd, 10);
	if(lestenr < 0)
	{
		std::cout << "error : listen" << std::endl;
		exit(0);
	}



	// std::vector<struct pollfd>	struct_fdsS;
	// struct pollfd fd;
    // fd.fd = socketfd;
	// fd.events = POLLOUT | POLLIN;
	// struct_fdsS.push_back(fd);
    // // fd_set current_socket, ready_socket;
    // // FD_ZERO(&current_socket);
    // // FD_SET(socketfd, &current_socket);

    // while (true)
    // {
    //     std::vector<struct pollfd>	struct_fdsC(struct_fdsS);
    //     // ready_socket = current_socket;
    //     if(poll(&struct_fdsC[0], struct_fdsC.size(), 600000) < 0)
    //     {
    //         exit(1);
    //     }
    //     for (size_t i = 0; i < struct_fdsC.size(); i++)
    //     {
    //         if(struct_fdsC[i].revents & POLLIN || struct_fdsC[i].revents & POLLOUT)
    //         {
    //             if(struct_fdsC[i].fd == socketfd)
    //             {
    //                 int			newfd;
	// 				newfd = accept(struct_fdsC[i].fd, (struct sockaddr *)NULL, NULL);
	// 				if( newfd < 0 )
	// 				{
	// 					std::cout << "	error	:	accpet	" << std::endl;	
	// 					exit(0);
	// 				}
    //                 struct pollfd fd;
    //                 fd.fd = newfd;
    //                 fd.events = POLLOUT | POLLIN;
    //                 struct_fdsS.push_back(fd);

    //             }
    //             else
    //             {
    //                 std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    //                 ssize_t		bytes;
	// 		        char		req[2024];
	// 		        std::string	ContentRequist;
    //                 bzero(req, 2024);
	// 				while((bytes = recv(struct_fdsC[i].fd, req, 2023, 0)) > 0)
	// 				{
	// 					ContentRequist.append(req);
	// 					bzero(req, 2024);
	// 					if (bytes < 2023)
	// 						break;
	// 				}
	// 				if(bytes < 0)
    //                 {
	// 					std::cout << "	error	:	byte	" << std::endl;	
	// 					exit(0);
    //                 }
    //                 std::cout << ContentRequist << std::endl;
    //                 write(struct_fdsC[i].fd, header.append("<h1> hello world</h1>").c_str(), header.length());
	// 				close(struct_fdsC[i].fd);
    //                 struct_fdsS.erase(struct_fdsS.begin() + i);
    //             }
    //         }
    //     }
        
    // }
    









	struct pollfd fd;
	std::vector<struct pollfd>	struct_fdsS;
    fd.fd = socketfd;
	fd.events = POLLOUT | POLLIN;
	struct_fdsS.push_back(fd);
	int pollValeu = 0;
	while (1)
	{
		std::vector<struct pollfd>	struct_fdsC(struct_fdsS);
		pollValeu = poll(&struct_fdsC[0], (unsigned int)struct_fdsC.size(), 6000000);
		if (pollValeu < 0)
		{
			std::cout << "error : poll" << std::endl;
			exit(1);
		}
		if (pollValeu == 0)
        {
			continue ;
        }
		for(size_t i = 0; i < struct_fdsC.size(); i++)
		{
			ssize_t		bytes;
			char		req[2024];
			std::string	ContentRequist;
			struct pollfd tmpstruct;
            std::cout << "POLLOUT : " << POLLOUT << std::endl;
            std::cout << "POLLIN : " << POLLIN << std::endl;
            std::cout << "struct_fdsC[i].revents : " << struct_fdsC[i].revents << std::endl;
			if (struct_fdsC[i].revents & POLLOUT || struct_fdsC[i].revents &  POLLIN)
			{
				if(struct_fdsC[i].fd == socketfd)
				{
					int			newfd;

					newfd = accept(struct_fdsC[i].fd, (struct sockaddr *)NULL, NULL);
					if( newfd < 0 )
					{
						std::cout << "	error	:	accpet	" << std::endl;	
						exit(0);
					}
					tmpstruct.fd = newfd;
					tmpstruct.events = POLLOUT | POLLIN;
					struct_fdsS.push_back(tmpstruct);
				}
                else
                {
                    ContentRequist.clear();
					bytes = 0;
                    std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                    bzero(req, 2024);
					while((bytes = recv(struct_fdsC[i].fd, req, 2023, 0)) > 0)
					{
						ContentRequist.append(req);
						bzero(req, 2024);
						if (bytes < 2023)
							break;
					}
					if(bytes < 0)
						exit(0);
                    write(struct_fdsC[i].fd, header.append("<h1> hello world</h1>").c_str(), header.length());
                    // std::cout << ContentRequist << std::endl;
					close(struct_fdsC[i].fd);
					// close(struct_fdsS[i].fd);
                    struct_fdsS.erase(struct_fdsS.begin() + i);

                }
            }
	    }
    }
}