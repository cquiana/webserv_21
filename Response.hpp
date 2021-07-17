#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "Http_config.hpp"
#include "Server_config.hpp"


class Response
{
private:
	Response();
public:
	Request const &_request;
	std::map<std::string, std::string> _headers;
	std::map<int, std::string> _errors;
	std::string _body;
	std::string _date;
	std::string _lastModif;
	std::string _CGIResponse;
    int _code;
    size_t _lenght;
//	Http_config* _http_config;
	Server_config _server_config;

	std::string _result;


//	Response(int code, Http_config* http_config);
	Response(int code, Server_config &server_config, const Request &request);


    ~Response();

    std::string responseToString();
    Response startGenerateResponse(); // serv conf

    void setHeaders(const std::string &key, const std::string &value);
    void setErrors();
	void setErrorCode(int code);
	void setDefaultHeader();
    void setDate();
    void setLastModif(const std::string &str);
	void setCGIResponse(const std::string &str);
    void setBody(std::string &body);
    void setContentLength(size_t len);

    int getErrorCode();

    std::string getHeader(const std::string &key) const;
    std::string getDate();

	void finishGenerateResponse();
	size_t getContetntLength();

	void errorPageFromFile(const std::string &path);
	void errorPageGenerator(int code);
	bool checkCGI();

	std::string generateCGI();
	bool generateGET(); // conf
	bool methodDELETE();
	void generateAutoindex();
	std::string getMimeType(const std::string &file);

	class FileCantOpenException:		public std::exception {
		virtual const char *what() const throw() ;
	};

};
#endif


