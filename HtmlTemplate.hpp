#ifndef WEBSERV_ERRORPATTERN_H
#define WEBSERV_ERRORPATTERN_H

#include <iostream>

class HtmlTemplate {
public:
	virtual std::string ShowErrorPage(const std::string &code, const std::string &textCode) = 0;
	virtual std::string ShowListPage() = 0;
};

#endif
