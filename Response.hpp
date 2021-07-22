#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include <dirent.h>
#include <sys/types.h>
#include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "Http_config.hpp"
#include "Server_config.hpp"
//#include "CGI.hpp"


class Response
{
private:
	Response();
public:
	Request &_request;
	std::map<std::string, std::string> _headers;
	std::map<int, std::string> _errors;
	std::string _body;
	std::string _date;
	std::string _lastModif;
	std::string _CGIResponse;
	std::string _fullPath;
    int _code;
    size_t _lenght;
//	Http_config* _http_config;
	Server_config _server_config;

	std::string _fullPath;


//	Response(int code, Http_config* http_config);
	Response(int code, Server_config &server_config, Request &request);


    ~Response();

    std::string responseToString();
    Response startGenerateResponse(); // serv conf

    void setFullPath();
    void setHeaders(const std::string &key, const std::string &value);
    void setErrors();
	void setErrorCode(int code);
	void setDefaultHeader();
    void setDate();
    void setLastModif(const std::string &str);
	void setCGIResponse(const std::string &str);
    void setBody(std::string &body);
    void setContentLength(size_t len);
    void setFullPath();

    std::string getFullPath();

    int getErrorCode();

    std::string getFullPath();
    std::string getHeader(const std::string &key) const;
    std::string getDate();

	void finishGenerateResponse();
	size_t getContetntLength();

	void errorPageFromFile(const std::string &path);
	void errorPageGenerator(int code);
	bool checkAllowedMethod();
	bool checkCGI();
	bool overloadClientMaxBodySize();

	std::string generateCGI();
	bool validMethod();
	bool generateGET(); // conf
	bool generatePOST();
	bool generatePUT();
	bool methodDELETE();
	void generatePUT();
	bool  generatePOST();
//	void generateListing(const std::string &path);
	std::string getMimeType(const std::string &file);

	class FileCantOpenException:		public std::exception {
		virtual const char *what() const throw() ;
	};

	void generateAutoindex(const std::string &path);


	void generateListing(const std::string &path);
};
#endif


