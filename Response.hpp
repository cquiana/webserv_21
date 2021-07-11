#pragma once
#include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"


class Response
{
private:

public:
	Request _request;
	std::map<std::string, std::string> _headers;
	std::map<int, std::string> _errors;
    std::string _protocol;
	std::string _path;

    std::string _statusText;
	std::string _body;
	std::string _date;
	std::string _lastModif;
    int _status;
    int _code;
    size_t _lenght;

	std::string _result;

    Response(/* args */);
	Response(int code);


    ~Response();

    std::string responseToString();
    Response startGenerateResponse(Request &request); // serv conf

    void setHeaders(const std::string &key, const std::string &value);
    void setErrors();
    void setDate();
    void setLastModif(const std::string &str);
    void setBody(std::string &body);

    std::string getHeader(std::string &key) const;

	void startGenerateResponse();
	void setErrorPage(int code);
	void errorPageFromFile(const std::string &path);
	bool isCGI();
	bool isAutoIndex();

	Response generateGET(const Request &request); // conf

};


