#include"Cgi.hpp"

Cgi::Cgi(const string& scriptName,const string& body)
{
	this->scriptName = scriptName;
	this->body = body;
}
Cgi::Cgi(const string& scriptName)
{
	this->scriptName = scriptName;
	this->body = "";
}
void Cgi::setEnvironment(const Request& header, const string &script_name)
{
	cout << header.getVersion() << endl;
	exit(1);
	this->env.push_back("SERVER_PROTOCOL=" + header.getVersion());
	this->env.push_back("REQUEST_METHOD=" + header.getMethod());
	this->env.push_back("PATH_INFO=" + this->scriptName);
	this->env.push_back("SCRIPT_NAME=" + script_name );
	this->env.push_back("QUERY_STRING=" + header.getQuery());
	this->env.push_back("QUERY_STRING=" + header.getQuery());
	this->env.push_back("CONTENT_TYPE=");
	this->env.push_back("SERVER_PORT=");
	this->env.push_back("PATH_TRANSLATED=");
	this->env.push_back("REMOTE_HOST=");
	this->env.push_back("REMOTE_ADDR=");
	this->env.push_back("AUTH_TYPE=");
	this->env.push_back("REMOTE_USER=");
	this->env.push_back("REMOTE_IDENT=");
	this->env.push_back("CONTENT_LENGTH=");
	this->env.push_back("SERVER_SOFTWARE=");
	this->env.push_back("SERVER_NAME=");
	this->env.push_back("GATEWAY_INTERFACE=");
}
string Cgi::HandelScript()
{
	int			forkValeu = 0;
	int			fd[2];
	string	ext;

	char **argv = new char*[3];
	memset(argv, 0, (3) * sizeof(char *));
	argv[0] = (char *)"./";
	argv[1] = (char *)scriptName.c_str();
	pipe(fd);
	forkValeu = fork();
	if(forkValeu == 0)
	{
		// dup2(fd[1],STDOUT_FILENO);
		// close(fd[0]);
		// if(execve(argv[0], (char *const *)argv, ) < 0)
		// {
		// 	cout << "execve filed" << endl;
		// 	exit(0);    
		// }
	}
	// waitpid(-1, 0, 0);
	String	respond;
	// char res[200];
	// bzero(res, 200);
	// ssize_t bytes = 0;
	// while((bytes = read(fd[0], res, 199)) != 0)
	// {
	// 	respond.append(res);
	// 	bzero(res, 200);
	// 	if (bytes < 199)
	// 		break;
	// }
	// close(fd[0]);
	// close(fd[1]);
	return respond;
}
Cgi::~Cgi()
{
}