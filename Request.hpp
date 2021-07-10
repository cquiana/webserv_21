#pragma once
// #include "Webserv.hpp"
#include "utils.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <string>
#include <iostream>

// class Client;
class Request
{
private:

	std::map<std::string, std::string> _headers;
	std::string _uri;
	std::string _queryString;
	std::string _reqBody;
	std::string _toRead;

	size_t _contentLength;
	bool _isComplete;
	bool _correctBody;
	int _sock;

	Request();


public:
	Request(int sock);
	virtual ~Request();

	void parseRequest(std::string request);
	void parseHeaders(std::string &request);
	void parseBody(std::string &request);
	void receive();
	size_t getContentLength();
	std::string const &getMethod();
	std::string getHost();
	std::string const &getReqBody();
	std::string getAuthType();
	std::string getConnection();
	std::string getUserAgent();
	std::string getUri();
	std::string getPath();
	std::string getQueryString();
	bool getCompete();
	bool checkHeader();

};

