#include "webserver.h"
#include <dirent.h>

bool	checkFile(String file)
{
	fstream new_file;
    
    new_file.open(file.c_str(), ios::in);
	if (new_file.is_open() == false)
    	return (false);
	return (true);
}

String*  getContentFile(String file)
{
    fstream new_file;
    
    new_file.open(file.c_str(), ios::in);
    if (new_file.is_open() == false)
        return (NULL);
    String  sa;
    String  *content = new String();
    while (getline(new_file, sa))
        content->append(sa + "\n");
    new_file.close();
    return (content);
}


String	getFileContent(const vector<String>& indexes, const String& path)
{
	String value;
	for (size_t i = 0; i < indexes.size(); i++)
	{
		String tmp(path);
		tmp.append(indexes.at(i));
		if (checkFile(tmp) == true)
			return (tmp);
	}
	return ("");
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
