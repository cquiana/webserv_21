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
	std::string _uri;
	std::string _queryString;
	std::string _reqBody;
	std::string _location;
	std::string _toRead;

	size_t _contentLength;
	bool _isComplete;
	bool _correctBody;
	int _sock;




public:
	Request(int sock);
	Request();
	virtual ~Request();

	std::map<std::string, std::string> _headers;

	void parseRequest(std::string request);
	void parseHeaders(std::string &request);
	void parseBody(std::string &request);
	bool receive();

	void setLocationPath();
	void setQueryString();
	void setContentLength();
	void setHeader(const std::string &key, const std::string &value);

	int getContentLength() ;
	const std::string & getMethod() const;
	std::string getHost();
	std::string  &getReqBody();
	std::string getAuthType();
	std::string getConnection();
	std::string getUserAgent();
	std::string getUri();
	std::string getPath() const;
	std::string getQueryString();
	const std::string getHttpVers() const;
	std::string getLocatinPath() const;
	bool getCompete() const;
	bool checkHeader() const;

	void eraseRequest();

	class ReadErrorException:			public std::exception {
		virtual const char *what() const throw() ;
	};

};
#endif

