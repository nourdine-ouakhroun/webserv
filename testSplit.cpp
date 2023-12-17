#include <iostream>
#include <vector>

std::vector<std::string> split(std::string line, std::string sep)
{
	std::vector<std::string> sp;
	if (line.empty())
		return (sp);
	size_t pos = 0;
	while ((pos = line.find(sep)) != std::string::npos)
	{
		std::string l = line.substr(0, pos);
		if (!l.empty())
			sp.push_back(l);
		pos += sep.length();
		line.erase(0, pos);
	}
	sp.push_back(line.substr(0, line.length()));
	return (sp);
}


int main( void )
{
    std::vector<std::string> s = split("Host: 10.11.2.8\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/114.0\nAccept: application/font-woff2;q=1.0,application/font-woff;q=0.9,*/*;q=0.8\nAccept-Language: en-GB,en;q=0.5\nAccept-Encoding: gzip, deflate\nDNT: 1\nConnection: keep-alive\nReferer: http://10.11.2.8/assets/css/fontawesome.css\nPragma: no-cache\nCache-Control: no-cache\nhello", ": ");
    for (std::vector<std::string>::iterator it = s.begin(); it != s.end(); it++)
    {
        std::cout << *it;
    }
    
    return 0;
}

// Host: 10.11.2.8
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/114.0
// Accept: application/font-woff2;q=1.0,application/font-woff;q=0.9,*/*;q=0.8
// Accept-Language: en-GB,en;q=0.5
// Accept-Encoding: gzip, deflate
// DNT: 1
// Connection: keep-alive
// Referer: http://10.11.2.8/assets/css/fontawesome.css
// Pragma: no-cache
// Cache-Control: no-cache
