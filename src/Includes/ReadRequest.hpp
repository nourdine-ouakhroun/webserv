#ifndef READREQUEST
#define READREQUEST

#include"SocketDependencies.hpp"
#include"Server.hpp"

#define NPOS  std::string::npos 
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

    void	    methodSerch();
    void        postUtils();
    int         headerMethod(String);
    void	    setOnlyHeadre();
    void	    putInString();
    void	    putInFile();
    void        removeBoundary(std::string &,size_t	&);
    void	    removePartOfupload();
    void        checkReqeust();
    void	    checkIfFile(size_t	&);
    void        Requist();
};


#endif