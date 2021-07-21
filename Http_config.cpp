//
// Created by demon on 23.06.2021.
//

#include "Http_config.hpp"
#include <vector>

Http_config::Http_config() : _client_max_body_size(-2), _active_server(false) {}

Http_config::~Http_config() {}

Http_config::Http_config(Http_config const &another) :
		_client_max_body_size(another._client_max_body_size), _servers(another._servers), _active_server(another._active_server){}

Http_config& Http_config::operator=(Http_config const &another) {
	_client_max_body_size = another._client_max_body_size;
	_servers = another._servers;
	_active_server = another._active_server;
	return *this;
}

bool Http_config::haveMaxBody() const {
	if (_client_max_body_size == -2)
		return false;
	else
		return true;
}

bool Http_config::haveServer(std::string servers_name) {
//	if (std::find(_servers_names.begin(), _servers_names.end(), servers_name) != _servers_names.end())
//		return true;
//	else
//		return false;
//
	for(std::vector<Server_config>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if ((*it).getName() == servers_name)
			return (true);
	}
	return (false);
}


bool Http_config::haveSomeServer() const {
	return (!(_servers.empty()));
}

bool Http_config::haveActiveServer() const {
	return (_active_server);
}

int Http_config::getMaxBody() const {
	return (_client_max_body_size);
}

Server_config Http_config::getServer(std::string servers_name) {
	for(std::vector<Server_config>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if ((*it).getName() == servers_name)
			return ((*it));
	}
	throw Http_config::ServerNotFoundException();
	// return (Server_config());
}

int Http_config::getActiveServer() const {
	if (!(_active_server))
		throw Http_config::ServerNotOpenedException();
	return (_servers.size() - 1);
}


void Http_config::setMaxBody(int max_body) {
	if (haveMaxBody())
		throw Http_config::MaxBodyAlreadySetException();
	else if (max_body < 0)
		throw Http_config::MaxBodyWrongSetException();
	else
		_client_max_body_size = max_body;
}

void Http_config::setMaxBodyToServers() {
	if (!(haveMaxBody()))
		throw Http_config::MaxBodyWrongSetException();
	for(std::vector<Server_config>::iterator it = _servers.begin(); it != _servers.end(); it++)
		(*it).setMaxBody(_client_max_body_size);
}

void Http_config::addServer() {
	if (_active_server) // -1 server can create, 0....1000000 current opened server
		throw Http_config::ServerAlreadyOpenedException();
	_servers.push_back(Server_config());
	_active_server = true;
}

void Http_config::checkLastServeer() {
	int n = _servers.size() - 1;
	if (n < 0)
		throw Http_config::SizeServersException();
	//std::cout << "Server ### " << _servers[n].getPort() << " : " << _servers[n].getRoot() << " : " << _servers[n].getName() << " : " << _servers[n].getIndex() << " : " << _servers[n].getAutoindex() << " : " << _servers[n].haveLocation() << " : " << _servers[n].getReturnCode() << " : " << _servers[n].getReturnArdess() << std::endl;
	//std::cout << "Server ### " << _servers[n].havePort() << " : " << _servers[n].haveRoot() << " : " << _servers[n].haveName() << std::endl;

	if (_servers[n].havePort() && _servers[n].haveName() && _servers[n].haveRoot())
	{
		//std::cout << "Server done" << std::endl; //ToDo enable for debug
		_active_server = false;
	}
	else if (_servers[n].havePort() && _servers[n].haveName() && _servers[n].haveReturnCode())
	{
		// std::cout << "Redirect server done" << std::endl; //ToDo enable for debug
		_active_server = false;
	}
	else
	{
		std::cout << "Server wrong : " << _servers[n].getPort() << " : " << _servers[n].getName() << " : " << _servers[n].getRoot() << " : " << _servers[n].getIndex() << " : " << _servers[n].getAutoindex() << " : " << _servers[n].haveLocation() << " : " << _servers[n].getReturnCode() << " : " << _servers[n].getReturnArdess() << std::endl;
		_servers.pop_back();
		_active_server = false;
		return;
	}
	_servers[n].initSocket();
}



const char *Http_config::PageAlreadySetException::what() const throw() {
	return ("EXCEPTION! Page Alraedy Set in this http...");
}

const char *Http_config::MaxBodyAlreadySetException::what() const throw() {
	return ("EXCEPTION! Max Body Alraedy Set in this http...");
};

const char *Http_config::MaxBodyWrongSetException::what() const throw() {
	return ("EXCEPTION! Max Body Wrong in this http...");
};

const char *Http_config::ServerExistException::what() const throw() {
	return ("EXCEPTION! Server Alraedy Set in this http...");
};

const char *Http_config::ServerNotFoundException::what() const throw() {
	return ("EXCEPTION! Server Not Found on this http...");
};

const char *Http_config::ServerNotOpenedException::what() const throw() {
	return ("EXCEPTION! Server Not Opened on this http...");
};

const char *Http_config::SizeServersException::what() const throw() {
	return ("EXCEPTION! Size of servers[] wrong...");
};

const char *Http_config::ServerAlreadyOpenedException::what() const throw() {
	return ("EXCEPTION! There are one opened server yeat...");
};

const char *Http_config::ServerSocketException::what() const throw() {
	return ("EXCEPTION! Server socket exception...");
};

