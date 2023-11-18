#ifndef SERVERMODEL_TPP
#define SERVERMODEL_TPP

template <typename T>
bool	ServerModel::findLocationByPath(std::vector<Location> locations, const String& rootPath,
						const String& srcPath, void (*to_do) (const Location&, T&), T& value)
{
	static std::vector<Location> tmplocations;
	if (tmplocations.empty() == true)
		tmplocations = locations;
	std::vector<Location>::iterator ibegin = locations.begin();
	std::vector<Location>::iterator iend = locations.end();
	while (ibegin < iend)
	{
		
		std::vector<Data> data = ibegin->getData("root");
		if (data.empty() == true)
			ibegin->addData(Data("root", rootPath));
		{
			std::vector<Data> roots = ibegin->getData("root");
			String root;
			if (roots.empty() == false)
				root = roots.at(0).getValue();
			if (root.empty() == false && ServerModel::checkIsDirectory(root.append(srcPath)) == 0)
			{
				value.append(readFile(root));
				return (true);
			}
		}
		String tmpPath(ibegin->getPath());
		{
			std::vector<Data> aliases = ibegin->getData("alias");
			for (size_t i = 0; i < aliases.size(); i++)
			{
				std::cout << "{ " << ibegin->getData("root").at(0).getValue() << " }" << std::endl;
				std::vector<String> values = String(aliases[i].getValue()).split();
				if (std::find(values.begin(), values.end(), srcPath) != values.end())
				{
					Location l = getLocationByPath(tmplocations, srcPath);
					if (l.getPath().empty() == false)
						return (to_do(l, value), true);
					return (to_do(*ibegin, value), true);
				}
			}
		}
		if (!srcPath.compare(tmpPath.trim(" \t\n\r")) && tmpPath.length() == srcPath.length())
			return (to_do(*ibegin, value), true);
		if (ibegin->getInnerLocation().empty() == false \
			&& findLocationByPath(ibegin->getInnerLocation(), ibegin->getData("root").at(0).getValue(), srcPath, to_do, value) == true)
			return (true);
		ibegin++;
	}
	return (false);
}

#endif