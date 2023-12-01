#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Parser.hpp"
#include "Checker.hpp"
#include "Server.hpp"
#include "ResponseHeader.hpp"
#include <dirent.h>

String  getContentFile(String file);
String	getRootPath(String	root, String path);
String	getAliasPath(String	aliasPath);
String	tryFiles(const std::vector<String>& files, const String& path);
String	getFileContent(const std::vector<String>& indexes, const String& path);
String	getDirectoryContent(const String& dirname, String path);


#endif