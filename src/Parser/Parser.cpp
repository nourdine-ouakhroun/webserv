#include "Parser.hpp"

/**
 * @brief	Parser Default Constructor.
*/
Parser::Parser( void )
{
}

/**
 * @brief	Parser Parametraise Constructor.
*/
Parser::Parser(const String& _fileName) : fileName(_fileName)
{
	// Logger::warn(std::cout, readFile(), "");
	getFileContent();
	checkSyntax();
	splitContentIntoServers();
	getFinalResualt();
	checkKeys();
}

/**
 * @brief	Parser Copy Constructor.
*/
Parser::Parser(const Parser& copy)
{
	*this = copy;
}

/**
 * @brief	Parser Desctructor.
*/
Parser::~Parser( void ) throw()
{
	fileContent.clear();
	serversContents.clear();
	servers.clear();
}

/**
 * @brief	Parser assignment operator.
 * @param	target the target you wanna to assign to current object.
 * @return	refrance to current object.
 * @exception	maybe throwing a bad_alloc exception.
*/
Parser& Parser::operator=(const Parser& target)
{
	if (this != &target)
	{
		fileName = target.fileName;
		servers = target.servers;
		fileContent = target.fileContent;
		serversContents = target.serversContents;
	}
	return (*this);
}

/**
 * @brief	Getter of vector of vector of String.
 * @param	(none)
 * @return	constant refrance vector of vector of String.
 * @exception	no-throw exception.
*/
const std::vector<std::vector<String> >&	Parser::getServersContents( void ) const
{
	return (serversContents);
}

/**
 * @brief	Getter of vector of Server Models.
 * @param	(none)
 * @return	constant refrance vector of Server Model.
 * @exception	no-throw exception.
*/
const	std::vector<ServerModel>&	Parser::getServers( void ) const
{
	return (servers);
}

/**
 * @brief	checking syntax errors like unclosed brackets or semicolon.
 * @param	(none)
 * @return 	(none)
 * @exception	throw ParsingException if there is a syntax error.
*/
void	Parser::checkSyntax( void )
{
	int	openBrackets = 0;
	std::vector<String> server;
	bool	insideServer = false;
	std::vector<String>::iterator iterBegin = fileContent.begin();
	std::vector<String>::iterator iterEnd = fileContent.end();

	while (iterBegin < iterEnd)
	{
		openBrackets += iterBegin->countRepeating('{');
		openBrackets -= iterBegin->countRepeating('}');
		if (!iterBegin->compare("server"))
		{
			insideServer = !insideServer;
			iterBegin++;
			continue ;
		}
		if (insideServer == true)
		{
			if (iterBegin->contains("location ") == true || iterBegin->countRepeating('{') || iterBegin->countRepeating('}'))
			{
				iterBegin++;
				continue ;
			}
			if (*(iterBegin->end() - 1) != ';')
			{
				String message;
				message.append("[emerg] invalid parameter \"").append(*iterBegin).append("\" in ").append(fileName);
				message.append(" test failed.");
				throw (ParsingException(message));
			}
		}
		iterBegin++;
	}
	if (openBrackets)
	{
		String message;
		message.append("webserv: [emerg] unexpected end of file, expecting \"");
		message.append((openBrackets < 0) ? "{" : "}").append("\" in ").append(fileName);
		message.append("\nwebserv: configuration file ").append(fileName).append(" test failed.");
		throw (ParsingException(message));
	}
}

/**
 * @brief	Split String into part Key and Value.
 * @param	line	String to split.
 * @return 	Data class that contains key and value.
 * @exception	no-throw exception.
*/
Data	Parser::extractDataFromString(String& line)
{
	std::vector<String> vec = line.split();
	String	key = vec.at(0).trim(" \t;");
	String	value("");
	for (std::vector<String>::size_type i = 1; i < vec.size(); i++)
		value.append(" ").append(vec.at(i).trim(" \t;"));
	return (Data(key, value.trim(" \t")));
}

/**
 * @brief	printing all Locations stored in Parser class.
 * @param	locs	const Location class.
 * @return	(none).
 * @exception	no-throw exception.
*/
void	Parser::printLocations(const Location& locs)
{
	static String s;
	if (locs.getAllData().empty() == true)
		return ;
	// printing Path of Location.
	std::cout << s << "Path : " << locs.getPath() << std::endl;

	// printing All data from GlobalModel (Key, Value).
	std::vector<Data> vec = locs.getAllData();
	for (std::vector<Location>::size_type i = 0; i < vec.size(); i++)
		Data::printData(vec.at(i), s);

	// printing All Location from GlobalModel (Key, Value).
	std::vector<Location> innerLoc = locs.getInnerLocation();
	for (std::vector<Location>::size_type i = 0; i < innerLoc.size(); i++)
	{
		s.append("\t");
		printLocations(innerLoc.at(i));
		s.erase(s.length() - 1);
	}
}

/**
 * @brief	Extract Location part from config file using recuresion.
 * @param	begin	The start of iterator.
 * @param	end		The end of iterator.
 * @return	new Location that extact from the config.
 * @exception	no-throw exception.
*/
Location	Parser::getLocations(std::vector<String>::iterator& begin, const std::vector<String>::iterator& end, String	path)
{
	std::vector<Location> newLocation;
	GlobalModel model;
	while (begin < end)
	{
		// if (!begin->compare("{"))
		// 	begin++;
		if (!begin->compare("}"))
		{
			begin++;
			break ;
		}
		if (!begin->compare(0, 9, "location "))
		{
			String _path = extractDataFromString(*begin).getValue();
			// maybe push_back throw an exception !!
			newLocation.push_back(getLocations(++begin, end, _path.trim(" \t")));
		}
		else
			model.addData(extractDataFromString(*(begin++)));
	}
	return (Location(model, path, newLocation));
}

String	Parser::readFile()
{
	String content;
	char	buffer[2];
	bool	isComment = false;

	int fd = open(fileName.c_str(), O_RDONLY);
	if (fd < 0)
		throw (ParsingException("Failed to open File."));
	while (1)
	{
		memset(buffer, 0, 2);
		ssize_t reader = read(fd, buffer, 1);
		if (reader != 1)
			break ;
		if (isComment == true && buffer[0] == '\n')
		{
			isComment = false;
			content.append(buffer);
		}
		else if (buffer[0] == '#')
		{
			isComment = true;
			content.append(buffer);
		}
		else if (isComment == false && (buffer[0] == '}' || buffer[0] == '{'))
		{
			content.append("\n");
			content.append(buffer);
			content.append("\n");
		}
		else if (isComment == false && (buffer[0] == ';'))
			content.append(";\n");
		else
			content.append(buffer);
	}
	close(fd);
	return (content);
}

/**
 * @brief	Read file content unsing std::ifstream and store it in vector container (fileContent).
 * @param	(none)
 * @return	(none)
 * @exception	throw ParsingException when failed to open file.
*/
void	Parser::getFileContent( void )
{
	String	tmp;
	size_t	pos;
	String	str = readFile();

	std::istringstream outfile(str);
	// read data from file and store it in vector of String.
	while (!outfile.eof())
	{
		std::getline(outfile, tmp, '\n');
		tmp.trim(" \t");
		if (tmp.size() == 0 || *tmp.begin() == '#')
			continue ;
		// this part for remove comment inside strings
		{
			pos = tmp.find('#');
			if (pos != String::npos)
				tmp.erase(pos);
		}
		// pushing data into vector
		fileContent.push_back(tmp);
	}
}

std::vector<String>	Parser::getServerConfig(std::vector<String>::iterator& iterBegin, const std::vector<String>::iterator& iterEnd)
{
	int	openBrackets = 0;
	std::vector<String> server;
	bool	insideServer = false;

	while (iterBegin < iterEnd)
	{
		if (!iterBegin->compare("server"))
		{
			insideServer = !insideServer;
			iterBegin++;
		}
		openBrackets += iterBegin->countRepeating('{');
		if (iterBegin->countRepeating('{'))
			iterBegin++;
		openBrackets -= iterBegin->countRepeating('}');
		if (!openBrackets)
			break ;
		if (insideServer == true)
		{
			String s = iterBegin->trim(" \t");
			server.push_back(s);
			iterBegin++;
		}
	}
	return server;
}

void	Parser::parsingFile(std::vector<String> content)
{
	ServerModel server;
	std::vector<String>::iterator iBegin = content.begin();
	std::vector<String>::iterator iEnd = content.end();
	while (iBegin < iEnd)
	{
		if (iBegin->compare(0, 9, "location ")) // Not A Location Part
		{
			server.addData(extractDataFromString(*iBegin));
			iBegin++;
		}
		else // Location Part
		{
			String _path = extractDataFromString(*iBegin).getValue();
			server.addLocation(getLocations(++iBegin, iEnd, _path.trim(" \t")));
			String str("");
			// Location::printAllLocations(server.getLocation(), str);
		}
	}
	servers.push_back(server);
}

void	Parser::splitContentIntoServers( void )
{
	std::vector<String>::iterator begin = fileContent.begin();
	const std::vector<String>::iterator end = fileContent.end();
	while (begin < end)
	{
		serversContents.push_back(getServerConfig(begin, end));
		begin++;
	}
}

void	Parser::printServerModel(const ServerModel& server)
{
	std::vector<Location>::const_iterator b = server.getLocation().begin();
	std::vector<Location>::const_iterator e = server.getLocation().end();
	while (b < e)
	{
		printLocations(*b);
		b++;
	}
}

void	Parser::getFinalResualt( void )
{
	std::vector<std::vector<String> >::iterator begin = serversContents.begin();
	std::vector<std::vector<String> >::iterator end = serversContents.end();
	while (begin < end)
	{
		parsingFile(*begin);
		begin++;
	}
}

std::vector<Data>	Parser::parseHeader(const String& header)
{
	std::vector<Data> vec;
	std::istringstream	iss(header);
	String			tmp, key, value;
	String			httpMethod;
	std::getline(iss, httpMethod, '\n');
	vec.push_back(Data("Method", httpMethod.trim(" \t")));
	unsigned long	pos = 0;
	while (iss.eof() == false)
	{
		std::getline(iss, tmp, '\n');
		if (tmp.length() == 0 || tmp[0] == '\r')
			continue ;
		if ((pos = tmp.find_first_of(':')) != String::npos)
		{
			key = tmp.substr(0, pos);
			value = tmp.substr(pos + 1);
			vec.push_back(Data(key.trim(" \t"), value.trim(" \t")));
		}
	}
	return (vec);
}

void	Parser::checkKeys( void )
{
	std::vector<String> keys = String(KEYS).split(' ');
	for (unsigned int i = 0; i < (unsigned int)servers.size(); i++)
	{
		std::vector<Data> _data = servers.at((unsigned int)i).getAllData();
		for (unsigned int j = 0; j < (unsigned int)_data.size(); j++)
			if (find(keys.begin(), keys.end(), _data.at(j).getKey()) == keys.end())
			{
				String message("Invalid Key '");
				message.append(_data.at(j).getKey()).append("'.");
				throw (ParsingException(message));
			}
	}
}

