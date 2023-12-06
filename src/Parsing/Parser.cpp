#include "Parser.hpp"

/**
 * @brief CONFIG FILE EXAMPLE :
 * 
 * Server {
 * 		listen	8080;
 * 		server_name	localhost;
 * 		location / {
 * 			root	html;
 * 			index	index.html;
 * 			location /world {
 * 				root	html;
 * 				index	world.html;
 * 			}
 * 		}
 * 		error_page	402 403 404 405 40x.html;
 * }
 * Server {
 * 		listen	80;
 * 		server_name	mehid.com;
 * 		location / {
 * 			root	html;
 * 			index	index.html;
 * 			location /world {
 * 				root	html;
 * 				index	world.html;
 * 			}
 * 		}
 * 		error_page	402 403 404 405 40x.html;
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @brief	AFTER GETFILECONTENT :
 * ~~~~~~~~~~~~~~~~~~~ std::vector<String> fileContent ~~~~~~~~~~~~~~~~~~~
 * [0] 	: "Server"
 * [1] 	: "{"
 * [2] 	: "listen	8080;"
 * [3] 	: "server_name	localhost;"
 * [4] 	: "location /"
 * [5] 	: "{"
 * [6] 	: "root	html;"
 * [7] 	: "index	index.html;"
 * [8] 	: "location /world"
 * [9] 	: "{"
 * [10] : "root	html;"
 * [11] : "index	world.html;"
 * [12] : "}"
 * [13] : "}"
 * [14] : "error_page	402 403 404 405 40x.html;"
 * [15] : "}"
 * [16] : "Server"
 * [17] : "{"
 * [18] : "listen	80;"
 * [19] : "server_name	mehdi.com;"
 * [20] : "location /"
 * [21] : "{"
 * [22] : "root	html;"
 * [23] : "index	index.html;"
 * [24] : "location /world"
 * [25] : "{"
 * [26] : "root	html;"
 * [27] : "index	world.html;"
 * [28] : "}"
 * [29] : "}"
 * [30] : "error_page	402 403 404 405 40x.html;"
 * [31] : "}"
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @brief	AFTER CHECKSYSNTAX :
 * 	check semicolon and remove it, if an error happen then this function thow an Exception.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * [0] 	: "Server"
 * [1] 	: "{"
 * [2] 	: "listen	8080"
 * [3] 	: "server_name	localhost"
 * [4] 	: "location /"
 * [5] 	: "{"
 * [6] 	: "root	html"
 * [7] 	: "index	index.html"
 * [8] 	: "location /world"
 * [9] 	: "{"
 * [10] : "root	html"
 * [11] : "index	world.html"
 * [12] : "}"
 * [13] : "}"
 * [14] : "error_page	402 403 404 405 40x.html"
 * [15] : "}"
 * [16] : "Server"
 * [17] : "{"
 * [18] : "listen	80"
 * [19] : "server_name	mehdi.com"
 * [20] : "location /"
 * [21] : "{"
 * [22] : "root	html"
 * [23] : "index	index.html"
 * [24] : "location /world"
 * [25] : "{"
 * [26] : "root	html"
 * [27] : "index	world.html"
 * [28] : "}"
 * [29] : "}"
 * [30] : "error_page	402 403 404 405 40x.html"
 * [31] : "}"
 */

/**
 * @brief	AFTER SPLIT CONTENT INTO SERVERS
 * ~~~~~~~~~~ std::vector<std::vector<String> > serversContents ~~~~~~~~~~
 * INDEX: 0
 * [0][0] 	: "Server"
 * [0][1] 	: "{"
 * [0][2] 	: "listen	8080"
 * [0][3] 	: "server_name	localhost"
 * [0][4] 	: "location /"
 * [0][5] 	: "{"
 * [0][6] 	: "root	html"
 * [0][7] 	: "index	index.html"
 * [0][8] 	: "location /world"
 * [0][9] 	: "{"
 * [0][10]	: "root	html"
 * [0][11]	: "index	world.html"
 * [0][12]	: "}"
 * [0][13]	: "}"
 * [0][14]	: "error_page	402 403 404 405 40x.html"
 * [0][15]	: "}"
 * INDEX: 1
 * [1][0]  : "Server"
 * [1][1]  : "{"
 * [1][2]  : "listen	80"
 * [1][3]  : "server_name	mehdi.com"
 * [1][4]  : "location /"
 * [1][5]  : "{"
 * [1][6]  : "root	html"
 * [1][7]  : "index	index.html"
 * [1][8]  : "location /world"
 * [1][9]  : "{"
 * [1][10] : "root	html"
 * [1][11] : "index	world.html"
 * [1][12] : "}"
 * [1][13] : "}"
 * [1][14] : "error_page	402 403 404 405 40x.html"
 * [1][15] : "}"
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @brief	AFTER GET FINAL RESUALT :
 * ~~~~~~~~~~~~~~~~~~ std::vector<ServerPattern> servers ~~~~~~~~~~~~~~~~~~~
 * INDEX: 0
 * 		{
 * 			std::vector<Data> data :
 * 				.........................
 * 				|	Keys	|	Values	|
 * 				-------------------------
 * 				|	listen	|	8080	|
 * 				|server_name| localhost |
 * 				|error_page |...40x.html|
 * 				-------------------------
 * 
 * 			std::vector<LocationPattern> location :
 * 				INDEX: 0
 * 				{
 * 					String Path : "/"
 * 
 *	 				std::vector<Data> data :
 * 						.........................
 * 						|	Keys	|	Values	|
 * 						-------------------------
 * 						|	root	|	html	|
 * 						|	index	| index.html|
 * 						-------------------------
 * 
 * 					std::vector<LocationPattern> location :
 * 						INDEX: 0
 * 						{
 * 							String Path :
 * 								"/world"
 * 	
 *	 						std::vector<Data> data :
 * 								.........................
 * 								|	Keys	|	Values	|
 * 								-------------------------
 * 								|	root	|	html	|
 * 								|	index	| index.html|
 * 								-------------------------
 * 	
 * 							std::vector<LocationPattern> location :
 * 								empty.					
 * 						}
 * 					
 * 				}
 * 		}
 * INDEX: 1
 * 		{
 * 			std::vector<Data> data :
 * 				.........................
 * 				|	Keys	|	Values	|
 * 				-------------------------
 * 				|	listen	|	 80 	|
 * 				|server_name| mehdi.com |
 * 				|error_page |...40x.html|
 * 				-------------------------
 * 
 * 			std::vector<LocationPattern> location :
 * 				INDEX: 0
 * 				{
 * 					String Path : "/"
 * 
 *	 				std::vector<Data> data :
 * 						.........................
 * 						|	Keys	|	Values	|
 * 						-------------------------
 * 						|	root	|	html	|
 * 						|	index	| index.html|
 * 						-------------------------
 * 
 * 					std::vector<LocationPattern> location :
 * 						INDEX: 0
 * 						{
 * 							String Path :
 * 								"/world"
 * 	
 *	 						std::vector<Data> data :
 * 								.........................
 * 								|	Keys	|	Values	|
 * 								-------------------------
 * 								|	root	|	html	|
 * 								|	index	| index.html|
 * 								-------------------------
 * 	
 * 							std::vector<LocationPattern> location :
 * 								empty.					
 * 						}
 * 					
 * 				}
 * 		}
*/

/**
 * @brief	AFTER	CHECKKEYS :
 * 	If there is unkonw keys this function'll thrown Exception.
*/

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
	/** @attention
	 * 		Read ^ top of the file for more detials about parsing part.
	 */
	getFileContent(); // Reading Content From File and put it in File Content String.
	checkSyntax(); // Checking syntax error and throwing an Exception when error exists.
	splitContentIntoServers();	// Split File Content into vector of vector of String.
	getFinalResualt();	// convert vector of vector String into vector of ServerModel.
	checkServerKeys(); // check is there is unknow Keys.
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
	// clear servers vector.
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
 * @brief	Reading File content char by char.
 * @return	String Content
*/
String	Parser::readFile()
{
	String content;
	char	buffer[2];
	bool	isComment = false;

	int fd = open(fileName.c_str(), O_RDONLY);
	// if open faild.
	if (fd < 0)
		throw (ParsingException("Failed to open File."));
	while (1)
	{
		// set buffer to zeroes.
		memset(buffer, 0, 2);
		ssize_t reader = read(fd, buffer, 1);
		if (reader != 1)
			break ;
		if (isComment == true && buffer[0] == '\n')
		{
			// end of comment.
			isComment = false;
			content.append(buffer);
		}
		else if (buffer[0] == '#')
		{
			// begin of comment
			isComment = true;
			content.append(buffer);
		}
		else if (isComment == false && (buffer[0] == '}' || buffer[0] == '{'))
		{
			// "location / {" == become ==> "location / \n{\n".
			content.append("\n");
			content.append(buffer);
			content.append("\n");
		}
		else if (isComment == false && (buffer[0] == ';'))
			content.append(";\n"); // go back to new line after a semicolon.
		else
			content.append(buffer); // append buffer.
	}
	close(fd); // close file descriptor.
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
	if (str.empty() == true)
		throw (ParsingException("Empty File."));
	std::istringstream outfile(str);
	// read data from file and store it in vector of String.
	while (!outfile.eof())
	{
		std::getline(outfile, tmp, '\n'); // read line by line.
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
	if (fileContent.empty() == true)
		throw (ParsingException("Invalid Content."));
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
	if (fileContent.empty() == true)
		return ;
	std::vector<String>::iterator iterBegin = fileContent.begin();
	const std::vector<String>::iterator iterEnd = fileContent.end();
	while (iterBegin < iterEnd)
	{
		// Counting Brackets.
		openBrackets += iterBegin->countRepeating('{');
		openBrackets -= iterBegin->countRepeating('}');
		// Skip Server Line.
		if (!iterBegin->compare("server"))
		{
			insideServer = !insideServer;
			iterBegin++;
			continue ;
		}
		if (insideServer == true)
		{
			// Skip Location Line.
			if (iterBegin->contains("location ") == true || iterBegin->countRepeating('{') || iterBegin->countRepeating('}'))
			{
				iterBegin++;
				continue ;
			}
			// Check semicolon in the end of line.
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
	// Check Brackets is different a zero.
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
 * @brief	Extract Server Configuration one by one from vector of File Content vector.
 * @param	iterBegin	The begin of File Content vector.
 * @param	iterEnd		The End of File Content vector.
 * @return	new vector that's contains one Server Configuration.
*/
std::vector<String>	Parser::getServerConfig(std::vector<String>::iterator& iterBegin, const std::vector<String>::iterator& iterEnd)
{
	int	openBrackets = 0; // Numebr of Brackets.
	std::vector<String> server; // Return Value;
	bool	insideServer = false;

	while (iterBegin < iterEnd)
	{
		if (!iterBegin->compare("server"))
		{
			insideServer = true;
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

/**
 * @brief	Split	File Content into vector of Servers.
 * 			for more detials, go top.
*/
void	Parser::splitContentIntoServers( void )
{
	std::vector<String>::iterator begin = fileContent.begin();
	const std::vector<String>::iterator end = fileContent.end();
	while (begin < end)
	{
		std::vector<String> srv = getServerConfig(begin, end);
		if (srv.empty() == false)
			serversContents.push_back(srv);
		begin++;
	}
	if (serversContents.empty() == true)
		throw (ParsingException("No Server to run."));
	// clear file content vector.
	fileContent.clear();
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
 * @brief	Extract Location part from config file using recuresion.
 * @param	begin	The start of iterator.
 * @param	end		The end of iterator.
 * @return	new Location that extact from the config.
 * @exception	no-throw exception.
*/
LocationPattern	Parser::getLocations(std::vector<String>::iterator& begin, const std::vector<String>::iterator& end, String	path)
{
	std::vector<LocationPattern> newLocation; // Return Value.
	LocationPattern model; // For Data vector.
	while (begin < end)
	{
		// If *begin equal } : the end of Location, skip this line and break.
		if (!begin->compare("}"))
		{
			begin++;
			break ;
		}
		// If *begin location } : new Location, skip this line and go in to recursion, else add data to Data vector (AGeneralPattern).
		if (!begin->compare(0, 9, "location "))
		{
			String _path = extractDataFromString(*begin).getValue();
			// maybe push_back throw an exception !!
			newLocation.push_back(getLocations(++begin, end, _path.trim(" \t")));
		}
		else
			model.addData(extractDataFromString(*(begin++)));
	}
	return (LocationPattern(model, path, newLocation));
}

/**
 * @brief	Separate between Location and Data, and store it in ServerModel.
 * @param	content The Server Part.
*/
void	Parser::parsingFile(std::vector<String> content)
{
	ServerPattern server;
	std::vector<String>::iterator iBegin = content.begin();
	std::vector<String>::iterator iEnd = content.end();
	while (iBegin < iEnd)
	{
		if (iBegin->compare(0, 9, "location ")) // Data Part.
		{
			server.addData(extractDataFromString(*iBegin));
			iBegin++;
		}
		else // Location Part
		{
			String _path = extractDataFromString(*iBegin).getValue();
			server.addLocation(getLocations(++iBegin, iEnd, _path.trim(" \t")));
			String str("");
		}
	}
	servers.push_back(server);
}

/**
 * 
*/
void	Parser::getFinalResualt( void )
{
	std::vector<std::vector<String> >::iterator begin = serversContents.begin();
	std::vector<std::vector<String> >::iterator end = serversContents.end();
	while (begin < end)
	{
		parsingFile(*begin);
		begin++;
	}
	serversContents.clear();
}


/**
 * @brief	Prant Server Model Infos.
 * @param	server	Server to print
*/
void	Parser::printServerModel(const ServerPattern& server)
{
	std::vector<LocationPattern>::const_iterator b = server.getLocation().begin();
	std::vector<LocationPattern>::const_iterator e = server.getLocation().end();
	while (b < e)
	{
		printLocations(*b);
		b++;
	}
}

/**
 * @brief	printing all Locations stored in Parser class.
 * @param	locs	const Location class.
 * @return	(none).
 * @exception	no-throw exception.
*/
void	Parser::printLocations(const LocationPattern& locs)
{
	static String s;
	if (locs.getAllData().empty() == true)
		return ;
	// printing Path of Location.
	std::cout << s << "Path : " << locs.getPath() << std::endl;

	// printing All data from AGeneralPattern (Key, Value).
	std::vector<Data> vec = locs.getAllData();
	for (std::vector<LocationPattern>::size_type i = 0; i < vec.size(); i++)
		Data::printData(vec.at(i), s);

	// printing All Location from AGeneralPattern (Key, Value).
	std::vector<LocationPattern> innerLoc = locs.getInnerLocation();
	for (std::vector<LocationPattern>::size_type i = 0; i < innerLoc.size(); i++)
	{
		s.append("\t");
		printLocations(innerLoc.at(i));
		s.erase(s.length() - 1);
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
			vec.push_back(Data(key.trim(" \t\r\n"), value.trim(" \t\r\n")));
		}
	}
	return (vec);
}

void    Parser::checkLocationKeys(const std::vector<LocationPattern>& loca, const std::vector<String>& keys)
{
    for (size_t i = 0; i < loca.size(); i++)
    {
		std::vector<Data> _data = loca.at((unsigned int)i).getAllData();
		for (unsigned int j = 0; j < (unsigned int)_data.size(); j++)
			if (find(keys.begin(), keys.end(), _data.at(j).getKey()) == keys.end())
			{
				String message("Invalid Key '");
				message.append(_data.at(j).getKey()).append("'.");
				throw (ParsingException(message));
			}
        if (loca.at(i).getInnerLocation().empty() == false)
            checkLocationKeys(loca.at(i).getInnerLocation(), keys);  
    }
}
/**
 * @brief	Check All Keys inside File config is exists.
*/
void	Parser::checkServerKeys( void )
{
	std::vector<String> keys = String(KEYS).split(' ');
	for (unsigned int i = 0; i < (unsigned int)servers.size(); i++)
	{
		std::vector<Data> _data = servers.at((unsigned int)i).getAllData();
		for (unsigned int j = 0; j < (unsigned int)_data.size(); j++)
		{
			if (find(keys.begin(), keys.end(), _data.at(j).getKey()) == keys.end())
			{
				String message("Invalid Key '");
				message.append(_data.at(j).getKey()).append("'.");
				throw (ParsingException(message));
			}
		}
		checkLocationKeys(servers.at(i).getLocation(), keys);
	}
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
const	std::vector<ServerPattern>&	Parser::getServers( void ) const
{
	return (servers);
}