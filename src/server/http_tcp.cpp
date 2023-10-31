#include"http_tcp.hpp"

namespace http{
	TcpServer::TcpServer()
	{
	}
	TcpServer::~TcpServer()
	{
	}
	int TcpServer::StartServer()
	{
		int socketfd = socket(AF_INET, SOCK_STREAM, 0);
		if(socketfd < 0)
		{
			std::cout << "error socketfd" << std::endl;
			return 1;
		}
		void TcpServer::closeServer()
    	{
        	close(m_socket);
        	close(m_new_socket);
        	exit(0);
    	}
		return 0;
	}
}
