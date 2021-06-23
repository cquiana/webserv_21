//
// Created by demon on 23.06.2021.
//

#include "Server_config.hpp"

Server_config::Server_config() :
	_port(-1), _name(""), _root(""), _index(""), _autoindex(-1), _return_code(-1), _return_adress("") {}

Server_config::~Server_config() {}

bool Server_config::havePort() const {
	if (_port != -1)
		return true;
	else
		return false;
}

bool Server_config::haveName() const {
	return (!(_name.empty()));
}

bool Server_config::haveRoot() const {
	return (!(_root.empty()));
}

bool Server_config::haveIndex() const {
	return (!(_index.empty()));
}

bool Server_config::haveAutoindex() const {
	if (_autoindex != -1)
		return true;
	else
		return false;
}

bool Server_config::haveLocation() const {
	// ToDo WTF ??
}

bool Server_config::haveReturnCode() const {
	if (_return_code != -1)
		return true;
	else
		return false;
}


int Server_config::getPort() const {
	return _port;
}

std::string Server_config::getName() const {
	return _name;
}

std::string Server_config::getRoot() const {
	return _root;
}

std::string Server_config::getIndex() const {
	return _index;
}

int Server_config::getAutoindex() const {
	return _autoindex;
}

int Server_config::getReturnCode() const {
	return _return_code;
}

std::string Server_config::getReturnArdess() const {
	return _return_adress;
}


std::string Server_config::getRootByLocation(std::string type) const {
	// ToDo WTF ?
}


void Server_config::setPort(int port) {
	if (havePort())
		throw Server_config::PortAlraedySetException();
	else
		_port = port;
}

void Server_config::setName(std::string name) {
	if (haveName())
		throw Server_config::NameAlraedySetException();
	else
		_name = name;
}

void Server_config::setRoot(std::string root) {
	if (haveRoot())
		throw Server_config::RootAlraedySetException();
	else
		_root = root;
}

void Server_config::setIndex(std::string index) {
	if (haveIndex())
		throw Server_config::IndexAlraedySetException();
	else
		_index = index;
}


void Server_config::setAutoindex(int autoindex) {
	if (haveAutoindex())
		throw Server_config::AutoindexAlraedySetException();
	else
		_autoindex = autoindex;
}

void Server_config::setReturnCode(int return_code, std::string return_adress) {
	if (haveReturnCode())
		throw Server_config::ReturnCodeAlraedySetException();
	_return_code = return_code;
	_return_adress = return_adress;
}


void setLocation() {
	// ToDo WTF ?
}



const char *Server_config::PortAlraedySetException::what() const throw() {
	return ("EXCEPTION! Port Alraedy Set in this server...");
};

const char *Server_config::NameAlraedySetException::what() const throw() {
	return ("EXCEPTION! Name Alraedy Set in this server...");
};

const char *Server_config::RootAlraedySetException::what() const throw() {
	return ("EXCEPTION! Root Alraedy Set in this server...");
};

const char *Server_config::IndexAlraedySetException::what() const throw() {
	return ("EXCEPTION! Index Alraedy Set in this server...");
};

const char *Server_config::AutoindexAlraedySetException::what() const throw() {
	return ("EXCEPTION! Autoindex Alraedy Set in this server...");
};

const char *Server_config::ReturnCodeAlraedySetException::what() const throw() {
	return ("EXCEPTION! Return Code Alraedy Set in this server...");
};
//const char *Server_config::CodePageNumberAlraedySetException::what() const throw() {
//	return ("EXCEPTION! Code Page Number Alraedy Set in this server...");
//};

