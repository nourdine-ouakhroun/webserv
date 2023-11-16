#include"Cgi.hpp"

Cgi::Cgi(std::string ScriptName)
{
    PathScript = ScriptName;
}
std::string Cgi::HandelScript()
{
    int forkValeu = 0;
    char beffur[2024] = {0};
	if(this->PathScript.substr(PathScript.find('.') + 1) == "cpp")
    {
        std::string cmd("/usr/bin/c++ ");
        cmd.append(PathScript).append(" && ./a.out && /bin/rm -f a.out");
        const char *argv[] = {"/bin/bash", "-c", (char*)cmd.c_str(),NULL};
        int fd[2];
        pipe(fd);
        std::cout << cmd << std::endl;
        forkValeu = fork();
        if(forkValeu == 0)
        {
            dup2(fd[1],STDOUT_FILENO);
            close(fd[0]);
            if(execve(argv[0], (char *const *)argv, NULL) < 0)
            {
                std::cout << "execve filed" << std::endl;
                exit(0);    
            }
        }
        else
        {
            wait(0);
            read(fd[0], beffur, 2023);
            close(fd[0]);
            close(fd[1]);
        }
    }
    else
        return ERROR_404;
    return beffur;
}
Cgi::~Cgi()
{
}