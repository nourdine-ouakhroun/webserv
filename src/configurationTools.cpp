#include "Parser.hpp"
#include "Checker.hpp"
#include "Server.hpp"
#include <dirent.h>

String  getContentFile(String file)
{
    std::fstream new_file;
    
    new_file.open(file, std::ios::in);
    if (new_file.is_open() == false)
        return ("");
    String  sa;
    String  content;
    while (std::getline(new_file, sa))
        content.append(sa).append("\n");
    new_file.close();
    return content;
}

String	getRootPath(String	root, String path)
{
	String file(root);
	file.append(path).append((file.end() - 1)[0] == '/' ? "" : "/");
	return (file);
}

String	getAliasPath(String	aliasPath)
{
	aliasPath.rightTrim("/");
	return (aliasPath.append("/"));
}

String	tryFiles(const std::vector<String>& files, const String& path)
{
	String value;
	for (size_t i = 0; i < files.size() - 1; i++)
	{
		String tmp(path);
		tmp.append(files.at(i));
		value = readFile(tmp);
		if (value.length() != 0)
			return (value);
	}
	return (value);
}

String	getFileContent(const std::vector<String>& indexes, const String& path)
{
	String value;
	for (size_t i = 0; i < indexes.size(); i++)
	{
		String tmp(path);
		tmp.append(indexes.at(i));
		value = readFile(tmp);
		if (value.length() != 0)
			return (value);
	}
	return (value);
}

String	getDirectoryContent(const String& dirname, String path)
{
	DIR	*dir = opendir(dirname.c_str());
	if (!dir)
		return "";
	String body("<h1>Index of ");
	body.append(path).append("</h1><hr><pre>");
	struct dirent *dirp;
	path.trim("/");
	while ((dirp = readdir(dir)) != NULL)
		body.append("<a href=\"")
			.append("").append(dirp->d_name)
			.append("\">").append(dirp->d_name)
			.append("</a><br>");
	body.append("</pre><hr>");
	closedir(dir);
	return (body);
}
