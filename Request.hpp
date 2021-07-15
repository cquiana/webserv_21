#ifndef REQUEST_HPP
# define REQUEST_HPP
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




public:
	Request(int sock);
	Request();
	virtual ~Request();

	void parseRequest(std::string request);
	void parseHeaders(std::string &request);
	void parseBody(std::string &request);
	bool receive();
	size_t getContentLength();
	const std::string &getMethod();
	std::string getHost();
	std::string  &getReqBody();
	std::string getAuthType();
	std::string getConnection();
	std::string getUserAgent();
	std::string getUri();
	std::string getPath();
	std::string getQueryString();
	std::string getHttpVers();
	bool getCompete() ;
	bool checkHeader();

};
#endif

