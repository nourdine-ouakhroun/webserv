#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "../Parsing/Parser.hpp"
#include "../Parsing/Checker.hpp"
#include "Server.hpp"
#include "ResponseHeader.hpp"
#include <dirent.h>
#include <map>

String*  getContentFile(String file);
String	getRootPath(String	root, String path);
String	getAliasPath(String	aliasPath);
String	tryFiles(const std::vector<String>& files, const String& path);
String	getFileContent(const std::vector<String>& indexes, const String& path);
String	getDirectoryContent(const String& dirname, String path);

bool	checkFile(String file);

#endif