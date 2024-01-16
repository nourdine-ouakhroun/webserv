#ifndef readRequest
#define readRequest

#include"Socket.hpp"
#include"Server.hpp"
#include <cstdio>

#define NPOS  string::npos 
#define GET 0
#define POST 1
#define DELET 2

#ifndef READ_NUMBER
#define READ_NUMBER 2025
#endif

class   ReadRequest
{
        Socket	&socket;
        ReadRequest();

    public:

        ReadRequest(Socket&);
        ~ReadRequest();

        void	methodSerch(string &);
        void	postUtils();
        int		headerMethod(String);
        void	setHeadre(string &);
        void	putInString();
        void	putInFile();
        void	removeBoundary(string &,size_t	&);
        void	removePartOfupload();
        void	checkReqeust();
        void	checkIfFile(size_t	&);
        void	handelChunked(string &);
        void	Request();
};


#endif