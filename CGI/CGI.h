#ifndef WEBSERV_21_CGI_H
#define WEBSERV_21_CGI_H

#include <iostream>
#include <cstdlib>
#include <cstring>

class CGI {
private:
	char**  _env;
	const std::string &_request;

	void pushEnv(const std::string &key, const std::string &value);
public:
	CGI(const std::string &request);
	~CGI();
	void Start();
};

#endif
