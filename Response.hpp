#pragma once
#include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"


class Response
{
private:

public:
	std::map<std::string, std::string> _headers;
	std::map<int, std::string> _errors;
    std::string _protocol;
    std::string _statusText;
	std::string _body;
	std::string _date;
	std::string _lastModif;
    int _status;
    size_t _lenght;

	std::string _result;

    Response(/* args */);


    ~Response();

    std::string responseToString();
    void startGenerateResponse(const Request &request); // serv conf

    void setHeaders(std::string &key, std::string &value);
    void setErrors();
    void setDate();
    void setLastModif(const std::string &str);

    std::string getHeader(std::string &key) const;

	void startGenerateResponse();
};


