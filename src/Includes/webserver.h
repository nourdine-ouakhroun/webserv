#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Parser.hpp"
#include "Checker.hpp"
#include "Server.hpp"
#include <dirent.h>
#include <map>

String	getFileContent(const vector<String>& indexes, const String& path);
String	getDirectoryContent(const String& dirname, String path);

bool    checkFile(String file);
unsigned int    convertStringToBinary(String str);


double      convertGigaToByte(double x);
double      convertMigaToByte(double x);
double      convertKiloToByte(double x);
long long   convertor(String str);
void        handleLogges(const GeneralPattern& data);


#endif