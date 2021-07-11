#ifndef WEBSERV_ERRORPATTERN_H
#define WEBSERV_ERRORPATTERN_H

#include <iostream>

class ErrorPattern {
public:
	virtual std::string ShowErrorPage(const std::string &code, const std::string &textCode) = 0;
};

#endif
