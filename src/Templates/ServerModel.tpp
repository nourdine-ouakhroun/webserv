#ifndef ServerPattern_TPP
#define ServerPattern_TPP

template <typename T>
bool	ServerPattern::findLocationByPath(vector<Location> locations, const String& rootPath,
						const String& srcPath, void (*to_do) (const Location&, T&), T& value)
{
	static vector<Location> tmplocations;
	if (tmplocations.empty() == true)
		tmplocations = locations;
	vector<Location>::iterator ibegin = locations.begin();
	vector<Location>::iterator iend = locations.end();
	while (ibegin < iend)
	{
		
		vector<Data> data = ibegin->getData("root");
		if (data.empty() == true)
			ibegin->addData(Data("root", rootPath));
		{
			vector<Data> roots = ibegin->getData("root");
			String root;
			if (roots.empty() == false)
				root = roots.at(0).getValue();
			if (root.empty() == false && ServerPattern::checkIsDirectory(root.append(srcPath)) == 0)
			{
				value.append(readFile(root));
				return (true);
			}
		}
		String tmpPath(ibegin->getPath());
		{
			vector<Data> aliases = ibegin->getData("alias");
			for (size_t i = 0; i < aliases.size(); i++)
			{
				vector<String> values = aliases[i].getValue().split();
				if (find(values.begin(), values.end(), srcPath) != values.end())
				{
					Location l = getLocationByPath(tmplocations, srcPath);
					if (l.getPath().empty() == false)
						return (to_do(l, value), true);
					return (to_do(*ibegin, value), true);
				}
			}
		}
		tmpPath.trim(" \t\n\r");
		if (srcPath.size() > 0)
			if (srcPath.at(srcPath.size() - 1) == '/' && tmpPath.at(tmpPath.size() - 1) != '/')
				tmpPath.append("/");
		
		if (!srcPath.compare(tmpPath) && tmpPath.length() == srcPath.length())
			return (to_do(*ibegin, value), true);
		if (ibegin->getInnerLocation().empty() == false \
			&& findLocationByPath(ibegin->getInnerLocation(), ibegin->getData("root").at(0).getValue(), srcPath, to_do, value) == true)
			return (true);
		ibegin++;
	}
	return (false);
}

#endif