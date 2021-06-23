//
// Created by demon on 23.06.2021.
//

#include "Http_config.hpp"
#include <vector>

Http_config::Http_config() : _client_max_body_size(-2) {}  // ToDo WTF ?											!!!!

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
//	if (std::find(_servers_names.begin(), _servers_names.end(), servers_name) != _servers_names.end())
//		return true;
//	else
//		return false;
//
	for(std::vector<const Server_config>::iterator it = _servers.begin(); it != _servers.end(); it++) // ToDo WTF const !!!!?????
	{
		if ((*it).getName() == servers_name)
			return (true);
	}
	return (false);
}


bool Http_config::haveSomeServer() const {
	return (!(_servers.empty()));
}


std::string Http_config::getErrorPage(int page) const {
	int n = 0;

	for(std::vector<const int>::iterator it = _error_page_ints.begin(); it != _error_page_ints.end(); it++) // ToDo WTF const !!!!?????
	{
		if ((*it) == page)
			return (_error_page_strings[n]);
		n++;
	}
	throw Http_config::ErrorPageNotExistException();
}

int Http_config::getMaxBody() const {
	return _client_max_body_size;
}

Server_config Http_config::getServer(std::string servers_name) const {
	// ToDo WTF ?																									!!!!
	for(std::vector<const Server_config>::iterator it = _servers.begin(); it != _servers.end(); it++) // ToDo WTF const !!!!?????
	{
		if ((*it).getName() == servers_name)
			return ((*it));
	}
	throw Http_config::ServerNotFoundException();
	// return (Server_config());
}


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

void Http_config::addServer() {
	// ToDo WTF ?					!!!!
	_servers.push_back(Server_config());
}

void Http_config::checkLastServeer() {
	int n = _servers.size();
	if (n <= 0)
		throw Server_config::SizeLocationsException();
	n--;
	if (_servers[n].havePort() && _servers[n].haveRoot() && !_servers[n].haveName())
		std::cout << "Server done" << std::endl;
	else
	{
		std::cout << "Server wrong : " << _servers[n].getPort() << " : " << _servers[n].getName()[0] << " : " << _servers[n].getRoot() << " : " << _servers[n].getIndex() << " : " << _servers[n].getAutoindex() << " : " << _servers[n].haveLocation() << " : " << _servers[n].getReturnCode() << " : " << _servers[n].getReturnArdess() << std::endl;
		_servers.pop_back();
	}
}



const char *Http_config::PageAlreadySetException::what() const throw() {
	return ("EXCEPTION! Page Alraedy Set in this http...");
}

const char *Http_config::MaxBodyAlreadySetException::what() const throw() {
	return ("EXCEPTION! Max Body Alraedy Set in this http...");
};

const char *Http_config::ServerExistException::what() const throw() {
	return ("EXCEPTION! Server Alraedy Set in this http...");
};

const char *Http_config::ErrorPageNotExistException::what() const throw() {
	return ("EXCEPTION! Error Page Not Exist in this http...");
};

const char *Http_config::ErrorPageAlreadyExistException::what() const throw() {
	return ("EXCEPTION! Error Page Alraedy Set in this http...");
};

const char *Http_config::ServerNotFoundException::what() const throw() {
	return ("EXCEPTION! Server Not Found on this http...");
};

const char *Http_config::SizeServersException::what() const throw() {
	return ("EXCEPTION! Size of servers[] wrong...");
};