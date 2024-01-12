#ifndef READREQUEST
#define READREQUEST

#include"SocketDependencies.hpp"
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
    SocketDependencies  &socket;
    ReadRequest();

public:

    ReadRequest(SocketDependencies&);
    ~ReadRequest();

    void	    methodSerch(string &Requist);
    void        postUtils();
    int         headerMethod(String);
    void	    setOnlyHeadre(string &Requist);
    void	    putInString();
    void	    putInFile();
    void        removeBoundary(string &,size_t	&);
    void	    removePartOfupload();
    void        checkReqeust();
    void	    checkIfFile(size_t	&);
    void        handelChunked(string &Requist);
    void        Requist();
};


#endif