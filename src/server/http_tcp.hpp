#ifndef HTTP_TCP_HPP
#define HTTP_TCP_HPP
#include<Sys/socket>
namespace http{

	class TcpServer
	{
		public :
			TcpServer();
			~TcpServer();
			int StartServer();
			void closeServer();
		private :

	};
}

#endif

