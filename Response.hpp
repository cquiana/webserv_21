#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "Http_config.hpp"


class Response
{
private:

public:
	Request _request;
	std::map<std::string, std::string> _headers;
	std::map<int, std::string> _errors;
	std::string _body;
	std::string _date;
	std::string _lastModif;
    int _code;
    size_t _lenght;
	Http_config* _http_config;

	std::string _result;

    Response(/* args */);
	Response(int code, Http_config* http_config);


    ~Response();

    std::string responseToString();
    Response startGenerateResponse(Request &request); // serv conf

    void setHeaders(const std::string &key, const std::string &value);
    void setErrors();
	void setErrorPage(int code);
	void setDefaultHeader();
    void setDate();
    void setLastModif(const std::string &str);
    void setBody(std::string &body);
    void setContentLength(size_t len);

    std::string getHeader(const std::string &key) const;
    std::string getDate();

	void startGenerateResponse();
	size_t getContetntLength();

	void errorPageFromFile(const std::string &path);
	bool checkCGI(const Request &request);
	bool isAutoIndex();
	Response generateCGI(const Request &request);

	Response generateGET(Request &request); // conf

};
#endif


