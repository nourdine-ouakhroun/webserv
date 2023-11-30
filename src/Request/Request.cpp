#include "Request.hpp"

std::string	readRequest(int clientFd)
{
	std::string	request;
	int			byte = 0;
	int			readbyte = 1;
	char		buf[readbyte + 1];
	while(1)
	{
		byte = read(clientFd, buf, readbyte);
		if (byte <= 0)
			break ;
		buf[byte] = 0;
		request += buf;
		if (byte < readbyte)
			break ;
	}
	return (request);
}

std::vector<std::string> split(std::string line, std::string sep)
{
	std::vector<std::string> sp;
	if (line.empty())
		return (sp);
	size_t pos = 0;
	while ((pos = line.find(sep)) != std::string::npos)
	{
		std::string l = line.substr(0, pos);
		if (!l.empty())
			sp.push_back(l);
		line = line.substr(pos + sep.length(), line.length());
	}
	sp.push_back(line.substr(0, line.length()));
	return (sp);
}

RequestLine Request::getRequestLine() const
{
	return(this->requestLine);
}
RequestHeader Request::getRequestHeader() const
{
	return(this->requestHeader);
}
RequestBody Request::getRequestBody() const
{
	return(this->requestBody);
}

// void Request::displayReq( int HorB )
// {
	// if	(HorB == 1)
	// {
	// 	std::cout << "---------------------- [ParseReq] ---------------------" << std::endl;
	// 	for(size_t i = 0; i < requestLine.size(); i++)
	// 		std::cout << requestLine[i] << std::endl;
	// }
	// if (HorB == 1)
	// {
	// 	std::cout << "-------------- [ Header ] ----------------" << std::endl;
	// 	for(maps::iterator it = headerRequest.begin(); it != headerRequest.end(); it++)
	// 		std::cout << it->first << "|" << it->second << std::endl;
	// }
	// else if (HorB == 2)
	// {
	// 	std::cout << "-------------- [ Body ] ----------------" << std::endl;
	// 	for(maps::iterator it = headerRequest.begin(); it != headerRequest.end(); it++)
	// 		std::cout << it->first << "|" << it->second << std::endl;
	// }

	// std::cout << "-------------- [ Body ] ----------------" << std::endl;
	// for(maps::iterator it = bodyRequest.begin(); it != bodyRequest.end(); it++)
	// 	std::cout << it->first << "|" << it->second << std::endl;
// }

int	Request::errorExit(int status, std::string errorMsg)
{
	if (status < 0)
	{
		std::cerr << errorMsg << std::endl;
		exit (1);
	}
	return (status);
}




// void Request::parseLine(std::string line)
// {
// 	size_t		pos = 0;
// 	std::string key;
// 	std::string value;
// 	if ((pos = line.find(": ")) != std::string::npos)
// 	{
// 		key   = line.substr(0, pos);
// 		value = line.substr(pos + 2, line.length());
// 	}
// 	headerRequest[key] = value;
// }

void Request::parseHeader(std::string h1)
{
	std::pair<std::string, std::string> keyValue;
	size_t	newlinePos = 0;
	size_t	pos = 0;
	std::string line;

	while ((newlinePos = h1.find("\n")) != std::string::npos)
	{
		line = h1.substr(0, newlinePos);
		// std::cout << "#" << 0 << " " << newlinePos << "#" << std::endl;
		if ((pos = line.find(": ")) != std::string::npos)
		{
			keyValue.first = line.substr(0, pos);
			keyValue.second = line.substr(pos + 2, line.length());
			
			requestHeader.set(keyValue); 
		}
		h1 = h1.substr(newlinePos + 1, h1.length());
	}
}

void    Request::parseRequest(std::string request)
{
	std::vector<std::string>	reqLine;
	std::string 	            headerReq;
	std::string		            bodyReq;

	size_t headerPos = request.find("\r\n\r\n");
	if (headerPos != std::string::npos)
	{
		size_t i = 0;
		size_t start = request.find("\r\n");
		if (start != std::string::npos)
		{
			// size_t i = start + 2;
			std::string line = request.substr(0, start);
			reqLine = split(line, " ");
			if (!reqLine.empty())
				this->requestLine.set(reqLine);
		}
		while (i < headerPos)
		{
			if (request[i] != '\r')
				headerReq += request[i];
			i++;
		}
		i = headerPos + 4;
		while (i < request.length())
		{
			if (request[i] != '\r')
				bodyReq += request[i];
			i++;
		}
	}
	parseHeader(headerReq);
}


	// std::cout << "#" << requestLine[0] << " " << requestLine[1] << " " << requestLine[2] << "#" << std::endl;
	// std::cout << "&" << headerReq << "&" << std::endl;
	// std::cout << "$" << bodyReq << "$" << std::endl;





	// if (methodeIs() == "GET")
	// {
	// 	size_t pos = 0;
	// 	std::string query;
	// 	if ((pos = requestLine[1].find("?")) != std::string::npos)
	// 	{
	// 		query = requestLine[1].substr(pos + 1, requestLine[1].length());

	// 		std::vector<std::string> content = split(query, "&");
	// 		if (!content.empty())
	// 		{
	// 			for (size_t i = 0; i < content.size(); i++)
	// 			{
	// 				std::vector<std::string> keyValue = split(content[i], "=");
	// 				if (!keyValue.empty())
	// 					this->query[keyValue[0]] = keyValue[1];
	// 			}
	// 			if (!this->query.empty())
	// 			{
	// 				for (maps::iterator it = this->query.begin(); it != this->query.end(); it++)
	// 				{
	// 					std::cout << it->first << " = " << it->second << std::endl;
	// 				}
	// 			}
	// 		}

	// 	}
	// }

	// else if (methodeIs() == "POST")
	// {
	// 	std::cout << "-------------- [ Body ] ----------------" << std::endl;
	// 	std::vector<std::string> contentType = split(headerRequest["Content-Type"], ";");
	// 	std::vector<std::string> boundray;
	// 	if (!contentType.empty())
	// 	{
	// 		if (contentType[0] == "multipart/form-data")
	// 		{
	// 			std::cout << "multipart/form-data" << std::endl;
	// 			boundray = split(contentType[1], "=");
	// 			if (!boundray.empty() && boundray.size() == 2)
	// 			{
	// 				std::string boundrayStart = "--" + boundray[1];
	// 				std::string boundrayEnd = boundrayStart + "--";


	// 				std::vector<std::string> ss = split(bodyReq, boundrayStart);
	// 				if (!ss.empty())
	// 				{
	// 					// std::cout << "|" << boundray[1] << "|" << std::endl;
	// 					// std::cout << "|" << boundrayStart << "|" <<std::endl;
	// 					for (size_t i = 0; i < ss.size(); i++)
	// 					{
	// 						std::cout << "content = " << ss[i] << std::endl;
	// 					}
						
	// 				}
					
	// 			}
	// 		}
	// 		else if (contentType[0] == "application/x-www-form-urlencoded")
	// 		{
	// 			std::cout << "application/x-www-form-urlencoded" << std::endl;
	// 			std::vector<std::string> content = split(bodyReq, "&");
	// 			if (!content.empty())
	// 			{
	// 				for (size_t i = 0; i < content.size(); i++)
	// 				{
	// 					std::vector<std::string> keyValue = split(content[i], "=");
	// 					if (!keyValue.empty())
	// 						bodyRequest[keyValue[0]] = keyValue[1];
	// 				}
	// 				for (maps::iterator it = bodyRequest.begin(); it != bodyRequest.end(); it++)
	// 				{
	// 					std::cout << "value = " << it->second << std::endl;
	// 				}
					
	// 			}
	// 		}
	// 	}
	// }
	// else if (methodeIs() == "DELETE")
	// {
	// }




	// std::cout << headerRequest["Content-Type"] << std::endl;
	// if (!contentType.empty())
	// {
	// 	std::cout << "POST" << std::endl;
		
	// 	std::cout << contentType[0] << std::endl;
	// 	if (contentType[0] == "multipart/form-data")
	// 	{
	// 		boundray = split(contentType[1], "=");
	// 		if (!boundray.empty())
	// 			std::cout << boundray[1] << std::endl;
	// 	}
	// 	else if (contentType[0] == "application/x-www-form-urlencoded")
	// 	{
	// 		// boundray = split(contentType[1], "=");
	// 		// if (!boundray.empty())
	// 			std::cout << contentType[0] << std::endl;
	// 	}
	// }
	// else
	// 	std::cout << "GET" << std::endl;
	// std::cout << "===> " << requestLine[1] << std::endl;

	// if (!contentType.empty())
	// 	boundray = split(contentType[1], "=");
	// 	if (!boundray.empty())
	// 		std::cout << boundray[1] << std::endl;


	// std::cout << bodyReq.find()
	// exit(1);
	// std::cout << "-------------- [ Body ] ----------------" << std::endl;
	// std::cout << bodyReq ;
	// displayReq();


	// requestIsFormed();
	// return (requestLine);
// }


bool	allowdSpecialchar(char chr)
{
	std::string specialChar = "_:;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%";
	for(size_t i = 0; i < specialChar.length(); i++)
	{
		if (chr == specialChar[i])
			return (1);
	}
	return (0);
}
bool	allowedChar(std::string url)
{
	std::cout << "|" << url << "|" << std::endl;
	for(size_t i = 0; i < url.length(); i++)
	{
		std::cout << (!isprint(url[i])) << std::endl;
		if (!isprint(url[i]))
			return (0);
	}
	return (1);
}
// void Request::statusCodeError(int statusCode, std::string phrase)
// {
// 	std::stringstream s;
// 	s << statusCode;
// 	std::string code;
// 	s >> code;

// 	std::string	response = "HTTP/1.1 " + code + " " + phrase + "\r\n\r\n" + code + " " + phrase;
// 	// std::cout << code << std::endl;
// 	// std::cout << response << std::endl;
// 	write(this->clientFd, response.c_str(), strlen(response.c_str()));
// 	close(this->clientFd);
// }
// void Request::requestIsFormed()
// {
// 	std::cout << "from methode requestIsformed()" << std::endl;
// 	// displayReq();
// 	if (requestLine.size() == 3 && !allowedChar(requestLine[1]))
// 		statusCodeError(400, "Bad Request!");
// 	else if (headerRequest["Transfer-Encoding"].empty() && headerRequest["Content-Length"].empty() && requestLine[0] == "POST")
// 		statusCodeError(400, "Bad Request!");
// 	else if (!headerRequest["Transfer-Encoding"].empty() && headerRequest["Transfer-Encoding"] != "chunked")
// 		statusCodeError(501, "Not Implimented!");
// 	else if (requestLine[1].length() > 2048)
// 		statusCodeError(414, "Request URI Too Longe!");
// 	else if (requestLine[1].length() > 2048)
// 		statusCodeError(413, "Content Too Large!");
// }

void Request::setMimeType()
{
	mimeType["*3gpp"] = "audio/3gpp";
	mimeType["*jpm"] = "video/jpm";
	mimeType["*mp3"] = "audio/mp3";
	mimeType["*rtf"] = "text/rtf";
	mimeType["*wav"] = "audio/wave";
	mimeType["*xml"] = "text/xml";
	mimeType["3g2"] = "video/3gpp2";
	mimeType["3gp"] = "video/3gpp";
	mimeType["3gpp"] = "video/3gpp";
	mimeType["ac"] = "application/pkix-attr-cert";
	mimeType["adp"] = "audio/adpcm";
	mimeType["ai"] = "application/postscript";
	mimeType["apng"] = "image/apng";
	mimeType["appcache"] = "text/cache-manifest";
	mimeType["asc"] = "application/pgp-signature";
	mimeType["atom"] = "application/atom+xml";
	mimeType["atomcat"] = "application/atomcat+xml";
	mimeType["atomsvc"] = "application/atomsvc+xml";
	mimeType["au"] = "audio/basic";
	mimeType["aw"] = "application/applixware";
	mimeType["bdoc"] = "application/bdoc";
	mimeType["bin"] = "application/octet-stream";
	mimeType["bmp"] = "image/bmp";
	mimeType["bpk"] = "application/octet-stream";
	mimeType["buffer"] = "application/octet-stream";
	mimeType["ccxml"] = "application/ccxml+xml";
	mimeType["cdmia"] = "application/cdmi-capability";
	mimeType["cdmic"] = "application/cdmi-container";
	mimeType["cdmid"] = "application/cdmi-domain";
	mimeType["cdmio"] = "application/cdmi-object";
	mimeType["cdmiq"] = "application/cdmi-queue";
	mimeType["cer"] = "application/pkix-cert";
	mimeType["cgm"] = "image/cgm";
	mimeType["class"] = "application/java-vm";
	mimeType["coffee"] = "text/coffeescript";
	mimeType["conf"] = "text/plain";
	mimeType["cpt"] = "application/mac-compactpro";
	mimeType["crl"] = "application/pkix-crl";
	mimeType["css"] = "text/css";
	mimeType["csv"] = "text/csv";
	mimeType["cu"] = "application/cu-seeme";
	mimeType["davmount"] = "application/davmount+xml";
	mimeType["dbk"] = "application/docbook+xml";
	mimeType["deb"] = "application/octet-stream";
	mimeType["def"] = "text/plain";
	mimeType["deploy"] = "application/octet-stream";
	mimeType["disposition-notification"] = "message/disposition-notification";
	mimeType["dist"] = "application/octet-stream";
	mimeType["distz"] = "application/octet-stream";
	mimeType["dll"] = "application/octet-stream";
	mimeType["dmg"] = "application/octet-stream";
	mimeType["dms"] = "application/octet-stream";
	mimeType["doc"] = "application/msword";
	mimeType["dot"] = "application/msword";
	mimeType["drle"] = "image/dicom-rle";
	mimeType["dssc"] = "application/dssc+der";
	mimeType["dtd"] = "application/xml-dtd";
	mimeType["dump"] = "application/octet-stream";
	mimeType["ear"] = "application/java-archive";
	mimeType["ecma"] = "application/ecmascript";
	mimeType["elc"] = "application/octet-stream";
	mimeType["emf"] = "image/emf";
	mimeType["eml"] = "message/rfc822";
	mimeType["emma"] = "application/emma+xml";
	mimeType["eps"] = "application/postscript";
	mimeType["epub"] = "application/epub+zip";
	mimeType["es"] = "application/ecmascript";
	mimeType["exe"] = "application/octet-stream";
	mimeType["exi"] = "application/exi";
	mimeType["exr"] = "image/aces";
	mimeType["ez"] = "application/andrew-inset";
	mimeType["fits"] = "image/fits";
	mimeType["g3"] = "image/g3fax";
	mimeType["gbr"] = "application/rpki-ghostbusters";
	mimeType["geojson"] = "application/geo+json";
	mimeType["gif"] = "image/gif";
	mimeType["glb"] = "model/gltf-binary";
	mimeType["gltf"] = "model/gltf+json";
	mimeType["gml"] = "application/gml+xml";
	mimeType["gpx"] = "application/gpx+xml";
	mimeType["gram"] = "application/srgs";
	mimeType["grxml"] = "application/srgs+xml";
	mimeType["gxf"] = "application/gxf";
	mimeType["gz"] = "application/gzip";
	mimeType["h261"] = "video/h261";
	mimeType["h263"] = "video/h263";
	mimeType["h264"] = "video/h264";
	mimeType["heic"] = "image/heic";
	mimeType["heics"] = "image/heic-sequence";
	mimeType["heif"] = "image/heif";
	mimeType["heifs"] = "image/heif-sequence";
	mimeType["hjson"] = "application/hjson";
	mimeType["hlp"] = "application/winhlp";
	mimeType["hqx"] = "application/mac-binhex40";
	mimeType["htm"] = "text/html";
	mimeType["html"] = "text/html";
	mimeType["ics"] = "text/calendar";
	mimeType["ief"] = "image/ief";
	mimeType["ifb"] = "text/calendar";
	mimeType["iges"] = "model/iges";
	mimeType["igs"] = "model/iges";
	mimeType["img"] = "application/octet-stream";
	mimeType["in"] = "text/plain";
	mimeType["ini"] = "text/plain";
	mimeType["ink"] = "application/inkml+xml";
	mimeType["inkml"] = "application/inkml+xml";
	mimeType["ipfix"] = "application/ipfix";
	mimeType["iso"] = "application/octet-stream";
	mimeType["jade"] = "text/jade";
	mimeType["jar"] = "application/java-archive";
	mimeType["jls"] = "image/jls";
	mimeType["jp2"] = "image/jp2";
	mimeType["jpe"] = "image/jpeg";
	mimeType["jpeg"] = "image/jpeg";
	mimeType["jpf"] = "image/jpx";
	mimeType["jpg"] = "image/jpeg";
	mimeType["jpg2"] = "image/jp2";
	mimeType["jpgm"] = "video/jpm";
	mimeType["jpgv"] = "video/jpeg";
	mimeType["jpm"] = "image/jpm";
	mimeType["jpx"] = "image/jpx";
	mimeType["js"] = "application/javascript";
	mimeType["json"] = "application/json";
	mimeType["json5"] = "application/json5";
	mimeType["jsonld"] = "application/ld+json";
	mimeType["jsonml"] = "application/jsonml+json";
	mimeType["jsx"] = "text/jsx";
	mimeType["kar"] = "audio/midi";
	mimeType["ktx"] = "image/ktx";
	mimeType["less"] = "text/less";
	mimeType["list"] = "text/plain";
	mimeType["litcoffee"] = "text/coffeescript";
	mimeType["log"] = "text/plain";
	mimeType["lostxml"] = "application/lost+xml";
	mimeType["lrf"] = "application/octet-stream";
	mimeType["m1v"] = "video/mpeg";
	mimeType["m21"] = "application/mp21";
	mimeType["m2a"] = "audio/mpeg";
	mimeType["m2v"] = "video/mpeg";
	mimeType["m3a"] = "audio/mpeg";
	mimeType["m4a"] = "audio/mp4";
	mimeType["m4p"] = "application/mp4";
	mimeType["ma"] = "application/mathematica";
	mimeType["mads"] = "application/mads+xml";
	mimeType["man"] = "text/troff";
	mimeType["manifest"] = "text/cache-manifest";
	mimeType["map"] = "application/json";
	mimeType["mar"] = "application/octet-stream";
	mimeType["markdown"] = "text/markdown";
	mimeType["mathml"] = "application/mathml+xml";
	mimeType["mb"] = "application/mathematica";
	mimeType["mbox"] = "application/mbox";
	mimeType["md"] = "text/markdown";
	mimeType["me"] = "text/troff";
	mimeType["mesh"] = "model/mesh";
	mimeType["meta4"] = "application/metalink4+xml";
	mimeType["metalink"] = "application/metalink+xml";
	mimeType["mets"] = "application/mets+xml";
	mimeType["mft"] = "application/rpki-manifest";
	mimeType["mid"] = "audio/midi";
	mimeType["midi"] = "audio/midi";
	mimeType["mime"] = "message/rfc822";
	mimeType["mj2"] = "video/mj2";
	mimeType["mjp2"] = "video/mj2";
	mimeType["mjs"] = "application/javascript";
	mimeType["mml"] = "text/mathml";
	mimeType["mods"] = "application/mods+xml";
	mimeType["mov"] = "video/quicktime";
	mimeType["mp2"] = "audio/mpeg";
	mimeType["mp21"] = "application/mp21";
	mimeType["mp2a"] = "audio/mpeg";
	mimeType["mp3"] = "audio/mpeg";
	mimeType["mp4"] = "video/mp4";
	mimeType["mp4a"] = "audio/mp4";
	mimeType["mp4s"] = "application/mp4";
	mimeType["mp4v"] = "video/mp4";
	mimeType["mpd"] = "application/dash+xml";
	mimeType["mpe"] = "video/mpeg";
	mimeType["mpeg"] = "video/mpeg";
	mimeType["mpg"] = "video/mpeg";
	mimeType["mpg4"] = "video/mp4";
	mimeType["mpga"] = "audio/mpeg";
	mimeType["mrc"] = "application/marc";
	mimeType["mrcx"] = "application/marcxml+xml";
	mimeType["ms"] = "text/troff";
	mimeType["mscml"] = "application/mediaservercontrol+xml";
	mimeType["msh"] = "model/mesh";
	mimeType["msi"] = "application/octet-stream";
	mimeType["msm"] = "application/octet-stream";
	mimeType["msp"] = "application/octet-stream";
	mimeType["mxf"] = "application/mxf";
	mimeType["mxml"] = "application/xv+xml";
	mimeType["n3"] = "text/n3";
	mimeType["nb"] = "application/mathematica";
	mimeType["oda"] = "application/oda";
	mimeType["oga"] = "audio/ogg";
	mimeType["ogg"] = "audio/ogg";
	mimeType["ogv"] = "video/ogg";
	mimeType["ogx"] = "application/ogg";
	mimeType["omdoc"] = "application/omdoc+xml";
	mimeType["onepkg"] = "application/onenote";
	mimeType["onetmp"] = "application/onenote";
	mimeType["onetoc"] = "application/onenote";
	mimeType["onetoc2"] = "application/onenote";
	mimeType["opf"] = "application/oebps-package+xml";
	mimeType["otf"] = "font/otf";
	mimeType["owl"] = "application/rdf+xml";
	mimeType["oxps"] = "application/oxps";
	mimeType["p10"] = "application/pkcs10";
	mimeType["p7c"] = "application/pkcs7-mime";
	mimeType["p7m"] = "application/pkcs7-mime";
	mimeType["p7s"] = "application/pkcs7-signature";
	mimeType["p8"] = "application/pkcs8";
	mimeType["pdf"] = "application/pdf";
	mimeType["pfr"] = "application/font-tdpfr";
	mimeType["pgp"] = "application/pgp-encrypted";
	mimeType["pkg"] = "application/octet-stream";
	mimeType["pki"] = "application/pkixcmp";
	mimeType["pkipath"] = "application/pkix-pkipath";
	mimeType["pls"] = "application/pls+xml";
	mimeType["png"] = "image/png";
	mimeType["prf"] = "application/pics-rules";
	mimeType["ps"] = "application/postscript";
	mimeType["pskcxml"] = "application/pskc+xml";
	mimeType["qt"] = "video/quicktime";
	mimeType["raml"] = "application/raml+yaml";
	mimeType["rdf"] = "application/rdf+xml";
	mimeType["rif"] = "application/reginfo+xml";
	mimeType["rl"] = "application/resource-lists+xml";
	mimeType["rld"] = "application/resource-lists-diff+xml";
	mimeType["rmi"] = "audio/midi";
	mimeType["rnc"] = "application/relax-ng-compact-syntax";
	mimeType["rng"] = "application/xml";
	mimeType["roa"] = "application/rpki-roa";
	mimeType["roff"] = "text/troff";
	mimeType["rq"] = "application/sparql-query";
	mimeType["rs"] = "application/rls-services+xml";
	mimeType["rsd"] = "application/rsd+xml";
	mimeType["rss"] = "application/rss+xml";
	mimeType["rtf"] = "application/rtf";
	mimeType["rtx"] = "text/richtext";
	mimeType["s3m"] = "audio/s3m";
	mimeType["sbml"] = "application/sbml+xml";
	mimeType["scq"] = "application/scvp-cv-request";
	mimeType["scs"] = "application/scvp-cv-response";
	mimeType["sdp"] = "application/sdp";
	mimeType["ser"] = "application/java-serialized-object";
	mimeType["setpay"] = "application/set-payment-initiation";
	mimeType["setreg"] = "application/set-registration-initiation";
	mimeType["sgi"] = "image/sgi";
	mimeType["sgm"] = "text/sgml";
	mimeType["sgml"] = "text/sgml";
	mimeType["shex"] = "text/shex";
	mimeType["shf"] = "application/shf+xml";
	mimeType["shtml"] = "text/html";
	mimeType["sig"] = "application/pgp-signature";
	mimeType["sil"] = "audio/silk";
	mimeType["silo"] = "model/mesh";
	mimeType["slim"] = "text/slim";
	mimeType["slm"] = "text/slim";
	mimeType["smi"] = "application/smil+xml";
	mimeType["smil"] = "application/smil+xml";
	mimeType["snd"] = "audio/basic";
	mimeType["so"] = "application/octet-stream";
	mimeType["spp"] = "application/scvp-vp-response";
	mimeType["spq"] = "application/scvp-vp-request";
	mimeType["spx"] = "audio/ogg";
	mimeType["sru"] = "application/sru+xml";
	mimeType["srx"] = "application/sparql-results+xml";
	mimeType["ssdl"] = "application/ssdl+xml";
	mimeType["ssml"] = "application/ssml+xml";
	mimeType["stk"] = "application/hyperstudio";
	mimeType["styl"] = "text/stylus";
	mimeType["stylus"] = "text/stylus";
	mimeType["svg"] = "image/svg+xml";
	mimeType["svgz"] = "image/svg+xml";
	mimeType["t"] = "text/troff";
	mimeType["t38"] = "image/t38";
	mimeType["tei"] = "application/tei+xml";
	mimeType["teicorpus"] = "application/tei+xml";
	mimeType["text"] = "text/plain";
	mimeType["tfi"] = "application/thraud+xml";
	mimeType["tfx"] = "image/tiff-fx";
	mimeType["tif"] = "image/tiff";
	mimeType["tiff"] = "image/tiff";
	mimeType["tr"] = "text/troff";
	mimeType["ts"] = "video/mp2t";
	mimeType["tsd"] = "application/timestamped-data";
	mimeType["tsv"] = "text/tab-separated-values";
	mimeType["ttc"] = "font/collection";
	mimeType["ttf"] = "font/ttf";
	mimeType["ttl"] = "text/turtle";
	mimeType["txt"] = "text/plain";
	mimeType["u8dsn"] = "message/global-delivery-status";
	mimeType["u8hdr"] = "message/global-headers";
	mimeType["u8mdn"] = "message/global-disposition-notification";
	mimeType["u8msg"] = "message/global";
	mimeType["uri"] = "text/uri-list";
	mimeType["uris"] = "text/uri-list";
	mimeType["urls"] = "text/uri-list";
	mimeType["vcard"] = "text/vcard";
	mimeType["vrml"] = "model/vrml";
	mimeType["vtt"] = "text/vtt";
	mimeType["vxml"] = "application/voicexml+xml";
	mimeType["war"] = "application/java-archive";
	mimeType["wasm"] = "application/wasm";
	mimeType["wav"] = "audio/wav";
	mimeType["weba"] = "audio/webm";
	mimeType["webm"] = "video/webm";
	mimeType["webmanifest"] = "application/manifest+json";
	mimeType["webp"] = "image/webp";
	mimeType["wgt"] = "application/widget";
	mimeType["wmf"] = "image/wmf";
	mimeType["woff"] = "font/woff";
	mimeType["woff2"] = "font/woff2";
	mimeType["wrl"] = "model/vrml";
	mimeType["wsdl"] = "application/wsdl+xml";
	mimeType["wspolicy"] = "application/wspolicy+xml";
	mimeType["x3d"] = "model/x3d+xml";
	mimeType["x3db"] = "model/x3d+binary";
	mimeType["x3dbz"] = "model/x3d+binary";
	mimeType["x3dv"] = "model/x3d+vrml";
	mimeType["x3dvz"] = "model/x3d+vrml";
	mimeType["x3dz"] = "model/x3d+xml";
	mimeType["xaml"] = "application/xaml+xml";
	mimeType["xdf"] = "application/xcap-diff+xml";
	mimeType["xdssc"] = "application/dssc+xml";
	mimeType["xenc"] = "application/xenc+xml";
	mimeType["xer"] = "application/patch-ops-error+xml";
	mimeType["xht"] = "application/xhtml+xml";
	mimeType["xhtml"] = "application/xhtml+xml";
	mimeType["xhvml"] = "application/xv+xml";
	mimeType["xm"] = "audio/xm";
	mimeType["xml"] = "application/xml";
	mimeType["xop"] = "application/xop+xml";
	mimeType["xpl"] = "application/xproc+xml";
	mimeType["xsd"] = "application/xml";
	mimeType["xsl"] = "application/xml";
	mimeType["xslt"] = "application/xslt+xml";
	mimeType["xspf"] = "application/xspf+xml";
	mimeType["xvm"] = "application/xv+xml";
	mimeType["xvml"] = "application/xv+xml";
	mimeType["yaml"] = "text/yaml";
	mimeType["yang"] = "application/yang";
	mimeType["yin"] = "application/yin+xml";
	mimeType["yml"] = "text/yaml";
	mimeType["zip"] = "application/zip";
}

Request::Request( void )
{
	this->setMimeType();
}
Request::Request( std::string request )
{
	// this->clientFd = clientFd;
	this->request = request;
	this->setMimeType();
}

Request::~Request( void )
{
}
