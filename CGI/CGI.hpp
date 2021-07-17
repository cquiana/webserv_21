#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <map>

class CGI {
private:
	char**  _env;
	std::map<std::string, std::string> _headers;

	void pushEnv(const std::string &key, const std::string &value);
public:
	CGI(std::map<std::string, std::string> headers);
	~CGI();
	void Start();
};

#endif
