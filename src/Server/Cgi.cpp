#include"Cgi.hpp"

Cgi::Cgi(std::string ScriptName)
{
    PathScript = ScriptName;
}
std::string Cgi::HandelScript()
{
    int forkValeu = 0;
    char beffur[2024] = {0};
	if(this->PathScript.substr(PathScript.find('.') + 1) == "c++")
    {
        const char *argv[] = {"/usr/bin/c++", (char*)PathScript.c_str(),NULL};
        forkValeu = fork();
        if(forkValeu == 0)
        {
            if(execve(argv[0], (char *const *)argv, NULL) < 0)
            {
                std::cout << "hello" << std::endl;
                exit(0);
            }
        }
        else
        {
            wait(0);
            int fd[2];
            pipe(fd);
            forkValeu = fork();
            if(forkValeu == 0)
            {
                dup2(fd[1],STDOUT_FILENO);
                close(fd[0]);
                if(execve("./a.out", NULL, NULL) < 0)
                {
                    std::cout << "hello" << std::endl;
                    exit(0);
                }
            }
            else
            {
                wait(0);
                forkValeu = fork();
                const char *argv[] = {"/bin/rm", "./a.out", NULL};
                if(forkValeu == 0)
                {
                    if(execve(argv[0], (char *const *) argv, NULL) < 0)
                    {
                        std::cout << "hello" << std::endl;
                        exit(0);
                    }
                }
                read(fd[0], beffur, 2023);
                close(fd[0]);
                close(fd[1]);
            }
        }
    }
    return beffur;
}
Cgi::~Cgi()
{
}