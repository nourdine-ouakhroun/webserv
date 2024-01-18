
		Request		req;
		Response	res;
		req.parseRequest(request);

		// std::cout << request << std::endl;
		// std::cout << req.getPathname() << std::endl;



		// std::string content;
		std::string	fileToServe;
		try {
			
			ServerPattern	server = ServerData::getServer(this->servers, req.header("Host"))[0];
			fileToServe = req.checkServer(server);

			res.setBody(readF(fileToServe));
		}
		catch (int status) {
			if (status == 404) {
				res.setStatusCode(status);
				res.setMsg("Not Found");
				res.setBody("<h1>Page Not Found</h1>");
				// std::cout << "Not found" << std::endl;
			}
			else if (status == 403) {
				res.setStatusCode(status);
				res.setMsg("Page Is Forbidden");
				res.setBody("<h1>Page Is Forbidden</h1>");
				// forbident
			}
			else if (status == 500) {
				res.setStatusCode(status);
				res.setMsg("Not Implamented");
				res.setBody("<h1>Not Implamented</h1>");
			}
			else if (status == 400) {
				res.setStatusCode(status);
				res.setMsg("Bad Request");
				res.setBody("<h1>Bad Request</h1>");
			}
			else if (status == 414) {
				res.setStatusCode(status);
				res.setMsg("Request-Uri Too Longe");
				res.setBody("<h1>Request-Uri Too Longe<h1>");
			}
			else if (status == 413) {
				res.setStatusCode(status);
				res.setMsg("Request Entity Too Longe");
				res.setBody("<h1>Request Entity Too Longe<h1>");
			}
			else if (status == 301) {
				res.setStatusCode(status);
				res.setMsg("Move Permanently");
				res.setHeader("location", req.getPathname() + "/");
				// res.setBody("<h1>Request Entity Too Longe<h1>");
			}
			// std::cout << status << "Error not found" << std::endl;
			// make Response With Status Code 
			// catch Exepction and make Response Error.
			// res.send(); // Send Response
		}

		std::ifstream fileStream(fileToServe, std::ifstream::ate | std::ifstream::binary);
		std::string	contentLength = std::to_string(fileStream.tellg());
		fileStream.close();

		res.setHeader("server", "nginx-v2");
		res.setHeader("Content-Type", res.getMimeType(extention(fileToServe)) );
		res.setHeader("Content-Length", contentLength);
		res.makeHeaderResponse();
		res.makeBodyResponse();

		std::string response = res.getResponse();
		ssize_t sendByte = 0;
		sendByte = write(working[i].fdpoll.fd, response.c_str(), response.length());
		response.erase(0, (size_t)sendByte);
		
		close(working[i].fdpoll.fd);
		erase(master, i);
	}