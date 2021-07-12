//
// Created by demon on 23.06.2021.
//

#include "Server_config.hpp"
#include "Location_config.hpp"

Server_config::Server_config() :
	_port(-1), _name(""), _root(""), _index(""), _autoindex(-1), _return_code(-1), _return_adress(""), _active_location(false) {}

Server_config::~Server_config() {}

Server_config::Server_config(Server_config const &another) :
	_port(another._port), _name(another._name), _root(another._root), _index(another._index), _autoindex(another._autoindex), _return_code(another._return_code), _return_adress(another._return_adress), _active_location(another._active_location) {}

Server_config& Server_config::operator=(Server_config const &another) {
	_port = another._port;
	_name = another._name;
	_root = another._root;
	_index = another._index;
	_autoindex = another._autoindex;
	_return_code = another._return_code;
	_return_adress = another._return_adress;
	_active_location = another._active_location;
	return *this;
}

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
	if (_locations.size() > 0)
		return true;
	else
		return false;
}

bool Server_config::haveReturnCode() const {
	if (_return_code > 0)
		return true;
	else
		return false;
}

bool Server_config::blockedReturnCode() const {
	if (_return_code == 0)
		return true;
	else
		return false;
}

bool Server_config::haveActiveLocation() const {
	return (_active_location);
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

std::string Server_config::getRootByLocation(std::string loc) {
	for(std::vector<Location_config>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		if ((*it).mIsPrefic() && (*it).haveRoot() && (*it).prefixCheck(loc))
			return ((*it).getRoot());
	}
	return (_root);
}

//std::string Server_config::getRootByLocation(std::string type, std::string loc) {
//	for(std::vector<Location_config>::iterator it = _locations.begin(); it != _locations.end(); it++)
//	{
//		if ((*it).haveType(type) && (*it).haveRoot())
//			return ((*it).getRoot());
//	}
//	for(std::vector<Location_config>::iterator it2 = _locations.begin(); it2 != _locations.end(); it2++)
//	{
//		if ((*it2).getLocationPrefix() == loc && (*it2).haveRoot())
//			return ((*it2).getRoot());
//	}
//	return (_root);
//}

int Server_config::getActiveLocation() const {
	if (_active_location)
		return (_locations.size() - 1);
	return (-1);
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
	setReturnCode(0, "");	// Block returnCode
}

void Server_config::setIndex(std::string index) {
	if (haveIndex())
		throw Server_config::IndexAlraedySetException();
	_index = index;
	setReturnCode(0, "");	// Block returnCode
}


void Server_config::setAutoindex(int autoindex) {
	if (haveAutoindex())
		throw Server_config::AutoindexAlraedySetException();
	_autoindex = autoindex;
	setReturnCode(0, "");	// Block returnCode
}

void Server_config::setReturnCode(int return_code, std::string return_adress) {
	if (haveReturnCode())
		throw Server_config::ReturnCodeAlraedySetException();
	else if (blockedReturnCode() && return_code != 0)
		throw Server_config::BlockedReturnCodeException();
	_return_code = return_code;
	_return_adress = return_adress;
}

void Server_config::addLocation(std::string location_path, std::string type) { // ToDo WTF ?				need test !!!!
	//Location_config nLocation(location_path, types);
	//_locations.push_back(Location_config(location_path, types));
	//setReturnCode(0, "");	// Block returnCode

	if (_active_location)
		throw Server_config::LocationNotOpenedException();
	// std::cout << "Location_config add: " << location_path << " - " << type << std::endl;
	_locations.push_back(Location_config(location_path, type));
	_active_location = true;
	setReturnCode(0, "");	// Block returnCode
}

void Server_config::checkLastLocation() {													// ToDo WTF ?				need test !!!!
	int n = _locations.size() - 1;
	if (n < 0)
		throw Server_config::SizeLocationsException();
	if (_locations[n].mIsCGI() && _locations[n].haveCgiPath() && !_locations[n].getType().empty())
	{
		std::cout << "CGI location done" << std::endl;
		_active_location = false;
	}
	else if (_locations[n].mIsPrefic() && _locations[n].haveRoot())
	{
		std::cout << "Prefix location done" << std::endl;
		_active_location = false;
	}
	else
	{
		std::cout << "location wrong : " << _locations[n].getLocationPrefix() << " : " << _locations[n].getType() << " : " << _locations[n].getMethods() << " : " << _locations[n].getRoot() << " : " << _locations[n].getCgiPath() << std::endl;
		_locations.pop_back();
		_active_location = false;
	}
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

const char *Server_config::BlockedReturnCodeException::what() const throw() {
    return ("EXCEPTION! Return Code Blocked by privios settings on this server...");
};

const char *Server_config::SizeLocationsException::what() const throw() {
	return ("EXCEPTION! Size of locations[] wrong...");
};

const char *Server_config::LocationNotOpenedException::what() const throw() {
	return ("EXCEPTION! Location Not Opened in this server...");
};


//const char *Server_config::CodePageNumberAlraedySetException::what() const throw() {
//	return ("EXCEPTION! Code Page Number Alraedy Set in this server...");
//};

