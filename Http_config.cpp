//
// Created by demon on 23.06.2021.
//

#include "Http_config.hpp"
#include <vector>

Http_config::Http_config() : _client_max_body_size(-2) {}  // ToDo WTF ?

Http_config::~Http_config() {}

bool Http_config::haveErrorPage(int page) const {
	if (std::find(_error_page_ints.begin(), _error_page_ints.end(), page) != _error_page_ints.end())
		return true;
	else
		return false;
}

bool Http_config::haveMaxBody() const {
	if (_client_max_body_size == -2)
		return false;
	else
		return true;
}

bool Http_config::haveServer(std::string servers_name) const {
	if (std::find(_servers_names.begin(), _servers_names.end(), servers_name) != _servers_names.end())
		return true;
	else
		return false;
}


bool Http_config::haveSomeServer() const {
	return (!(_servers_names.empty()));
}


std::string Http_config::getErrorPage(int page) const {
	int n = 0;

	for(std::vector<int>::iterator it = _error_page_ints.begin(); it != _error_page_ints.end(); it++)
	{
		if ((*it) = page)
			return (_error_page_strings[n]);
		n++;
	}
	throw Http_config::ErrorPageNotExistException();
}

int Http_config::getMaxBody() const {
	return _client_max_body_size;
}

//Server_config* Http_config::getServer(std::string servers_name) const {
//	// ToDo WTF ?
//}


void Http_config::setErrorPage(int error_page_int, std::string error_page_string) {
	if (haveErrorPage(error_page_int))
		throw Http_config::ErrorPageAlreadyExistException();
	_error_page_ints.push_back(error_page_int);
	_error_page_strings.push_back(error_page_string);
}

void Http_config::setMaxBody(int max_body) {
	if (haveMaxBody())
		throw Http_config::MaxBodyAlreadySetException();
	else
		_client_max_body_size = max_body;
}

//int Http_config::setServer(std::string servers_name, Server_config serverConfig); // ToDo WTF ?





const char *Http_config::PageAlreadySetException::what() const throw() {
	return ("EXCEPTION! Page Alraedy Set in this server...");
};

const char *Http_config::MaxBodyAlreadySetException::what() const throw() {
	return ("EXCEPTION! Max Body Alraedy Set in this server...");
};

const char *Http_config::ServerExistException::what() const throw() {
	return ("EXCEPTION! Server Alraedy Set in this server...");
};

const char *Http_config::ErrorPageNotExistException::what() const throw() {
	return ("EXCEPTION! Error Page Not Exist in this server...");
};

const char *Http_config::ErrorPageAlreadyExistException::what() const throw() {
	return ("EXCEPTION! Error Page Alraedy Set in this server...");
};