#ifndef readRequest
#define readRequest

#include"Socket.hpp"
#include"Server.hpp"
#include <cstdio>
#include"../Request/Request.hpp"

#define NPOS  string::npos 

#ifndef READ_NUMBER
#define READ_NUMBER 100000
#endif

class   ReadRequest
{
        Socket	&socket;
        string request;
        ReadRequest();

    public:

        ReadRequest(Socket&);
        ~ReadRequest();

		void 	recvSomthing(char *, size_t);
        void	setHeader(string &);
        void	checkReqeust();
        void	handelChunked();
        void	Read();

        const string& getRequest() const;

        class ReadException : exception
        {
            string exception_msg;

            public :
                ReadException();
                ReadException(const string &);
                const char * what() const throw();
                ~ReadException() throw();
        };
};


#endif