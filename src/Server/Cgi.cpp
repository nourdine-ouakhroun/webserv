#include"Cgi.hpp"

Cgi::Cgi(std::string ScriptName)
{
	PathScript = ScriptName;
	if(this->PathScript.substr(PathScript.find_last_of('.')) == ".cgi")
	{
		std::string cmd("./");
		MultCGI[".cgi"] = cmd;
	}
	if(this->PathScript.substr(PathScript.find_last_of('.')) == ".py")
	{
		std::string cmd("/usr/bin/python3");
		MultCGI[".py"] = cmd;
	}
	if(this->PathScript.substr(PathScript.find_last_of('.')) == ".php")
	{
		std::string cmd("/usr/bin/php");
		MultCGI[".php"] = cmd;
	}
}
std::string Cgi::HandelScript()
{
	int			forkValeu = 0;
	int			fd[2];
	std::string	ext;

	ext = this->PathScript.substr(PathScript.find_last_of('.'));
	if(MultCGI[ext].size() == 0)
		return ERROR_404;
	const char *argv[] = {(char*)MultCGI[ext].c_str(), (char*)PathScript.c_str(),NULL};
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
	// /**
	//  * @attention while read
	// */
	waitpid(-1, 0, 0);
	String						respond;
	char res[200];
	bzero(res, 200);
	ssize_t bytes = 0;
	while((bytes = read(fd[0], res, 199)) != 0)
	{
		respond.append(res);
		bzero(res, 200);
		if (bytes < 199)
			break;
	}
	close(fd[0]);
	close(fd[1]);
	return respond;
}
Cgi::~Cgi()
{
}