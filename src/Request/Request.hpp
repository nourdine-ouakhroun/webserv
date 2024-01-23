#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/stat.h>
#include "Response.hpp"
#include "ServerData.hpp"
#include "String.hpp"

typedef std::map<std::string, std::string> maps;

class Response;
class Request
{
    private:
        ServerPattern server;
        LocationPattern location;

        std::string method;
        std::string url;
        std::string version;
        std::string pathname;
        std::string query;
        
        maps        _header;

        std::string _request;
        std::string _body;

    public:
        Request( void );
        ~Request( void );

        void                        parseRequest(const std::string &request);
        void                        parseRequestLine( std::string reqLine );
        void                        parseHeader( void );
        void                        parseBody( void );
        void                        parseUrl( void );

        std::string header(const std::string &key);
        std::string extention( const std::string &path) const;

        void                setServer(const ServerPattern &server);

        const ServerPattern &getServer(void) const;
        const std::string   &getMethod( void ) const;
        const std::string   &getUrl( void ) const;
        const std::string   &getVersion( void ) const;
        const std::string   &getPathname( void ) const;
        const std::string   &getQuery( void ) const;

        const std::string   &getRequest(void) const;
        const std::string   &getBody(void) const;



        // Server
        // std::string checkServer( void );
        std::string getFullPath();
        std::string getErrorFile(int statusCode) const;
        string      getRoot() const;
        string      isFound(const string &path) const;
        bool        isCgi();
        string      getCgiFile();

            // void getPath();

            void
            isFormed();
        void isMatched(Response &res);
        void isRedirected(Response &res);
        void isMethodAllowed();
        void whichMethode(Response &res);

        static std::string readFile(const std::string &path);
        int isDirectory(const std::string &path) const;

        void setLocation(const LocationPattern &location);
        const LocationPattern &getLocation(void) const;

        void GetMethod(Response &res);
        void PostMethod(Response &res);
        void DeleteMethod(Response &res);
        };
// std::std::string	readRequest(FileDepandenc &file);
std::vector<std::string> split(std::string line, std::string sep);

#endif
