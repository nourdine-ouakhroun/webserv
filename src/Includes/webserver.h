#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

// #include "Directives.hpp"
#include "../Parsing/Parser.hpp"
#include "../Parsing/Checker.hpp"
#include "Server.hpp"
#include "ResponseHeader.hpp"
#include <dirent.h>
#include <map>

String*  getContentFile(String file);
String	getRootPath(String	root, String path);
String	getAliasPath(String	aliasPath);
String	tryFiles(const vector<String>& files, const String& path);
String	getFileContent(const vector<String>& indexes, const String& path);
String	getDirectoryContent(const String& dirname, String path);
void	handleLogges(GeneralPattern& server);
ResponseHeader	errorPage(vector<Data> errorPages, String errorNumber, String message);
ResponseHeader	autoIndexing(GeneralPattern& loca, const String& dir, String	path);
ResponseHeader	returnDirective(const vector<Data> &returns, ResponseHeader &responseHeader);

bool	checkFile(String file);

ResponseHeader	handler(ServerPattern& server, GeneralPattern __unused &model);
ResponseHeader	to_do(GeneralPattern& __unused targetInfo, __unused String path, String&);

#endif