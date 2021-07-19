#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <map>
# include <unistd.h>

class CGI {
private:
	char**  _env;
	std::map<std::string, std::string> _headers;
	const std::string &_cgi_path;
	const std::string &_cgi_ext;

	void pushEnv(const std::string &key, const std::string &value);
public:
	CGI(std::map<std::string, std::string> headers, const std::string &cgi_path, const std::string &cgi_ext);
	~CGI();
	const std::string &startCGI();
};

#endif
