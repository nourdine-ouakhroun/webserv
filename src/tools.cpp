#include "Server.hpp"
#include "Logger.hpp"
#include "Directives.hpp"
#include "Socket.hpp"

String	readFile(const String& path)
{
	String content;
	char	buffer[2];

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return (content);
	while (1)
	{
		memset(buffer, 0, 2);
		ssize_t reader = read(fd, buffer, 1);
		content.append(buffer);
		if (reader != 1)
			break ;
	}
	close(fd);
	return (content);
}

unsigned int convertStringToBinary(String str)
{
	unsigned int res = 0;
	vector<String> strs = str.split('.');
	for (size_t i = 0; i < strs.size(); i++)
		res += (unsigned int)strtol(strs[i].c_str(), NULL, 10) << i * 8;
	return (res);
}

unsigned short	getPort(String	value)
{
	unsigned short port = 80;
	String::size_type pos = value.find_last_of(':');
	if (pos != String::npos)
		port = (unsigned short)strtol(value.substr(pos + 1).c_str(), NULL, 10);
	return port;
}

double    convertKiloToByte(double x)
{
    return (x * 1000);
}
double    convertMigaToByte(double x)
{
    return (convertKiloToByte(x) * 1000);
}
double    convertGigaToByte(double x)
{
    return (convertMigaToByte(x) * 1000);
}

long long  convertor(String str)
{
    double value = 1e6;
    char *res = NULL;
    double x = strtod(str.c_str(), &res);
    if (res)
    {
        if (res[0] == 'G' || res[0] == 'g')
            value = convertGigaToByte(x);
        else if (res[0] == 'M' || res[0] == 'm')
            value = convertMigaToByte(x);
        else if (res[0] == 'K' || res[0] == 'k')
            value = convertKiloToByte(x);
    }
	else
		value = x;
    return (static_cast<long long>(value));
}
