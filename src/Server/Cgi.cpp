#include"Cgi.hpp"

Cgi::Cgi(std::string ScriptName)
{
	PathScript = ScriptName;
	if(this->PathScript.substr(PathScript.find('.')) == ".cpp")
	{
		std::string cmd("/usr/bin/c++ ");
		cmd.append(PathScript).append(" && ./a.out && /bin/rm -f a.out");
		MultCGI[".cpp"] = cmd;
	}
	if(this->PathScript.substr(PathScript.find('.')) == ".c")
	{
		std::string cmd("/usr/bin/gcc ");
		cmd.append(PathScript).append(" && ./a.out && /bin/rm -f a.out");
		MultCGI[".c"] = cmd;
	}
	if(this->PathScript.substr(PathScript.find('.')) == ".py")
	{
		std::string cmd("/usr/bin/python3 ");
		cmd.append(PathScript);
		MultCGI[".py"] = cmd;
	}
	if(this->PathScript.substr(PathScript.find('.')) == ".php")
	{
		std::string cmd("/usr/bin/php ");
		cmd.append(PathScript);
		MultCGI[".php"] = cmd;
	}
}
std::string Cgi::HandelScript()
{
	int			forkValeu = 0;
	char		beffur[2024] = {0};
	int			fd[2];
	std::string	ext;

	ext = this->PathScript.substr(PathScript.find('.'));
	if(MultCGI[ext].size() == 0)
		return ERROR_404;
	const char *argv[] = {"/bin/bash", "-c", (char*)MultCGI[ext].c_str(),NULL};
	pipe(fd);
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
	waitpid(-1, 0, 0);
	read(fd[0], beffur, 2023);
	close(fd[0]);
	close(fd[1]);
	return beffur;
}
Cgi::~Cgi()
{
}