//
// Created by demon on 23.06.2021.
//

#include "Server_config.hpp"
#include "Location_config.hpp"
#include "Webserv.hpp"

Server_config::Server_config() :
	_port(-1), _name(""), _root(""), _index(""), _autoindex(-1), _return_code(-1), _return_adress(""), _active_location(false), _socket(-1) {}

Server_config::~Server_config() {}

Server_config::Server_config(Server_config const &another) :
	_port(another._port), _name(another._name), _root(another._root), _index(another._index), _autoindex(another._autoindex), _return_code(another._return_code), _return_adress(another._return_adress), _active_location(another._active_location), _locations(another._locations), _socket(another._socket) {}

Server_config& Server_config::operator=(Server_config const &another) {
	_port = another._port;
	_name = another._name;
	_root = another._root;
	_index = another._index;
	_autoindex = another._autoindex;
	_return_code = another._return_code;
	_return_adress = another._return_adress;
	_active_location = another._active_location;
	_locations = another._locations;
	_socket = another._socket;
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

bool Server_config::haveSocket() const {
	if (_socket != -1)
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

int Server_config::getSocket() const {
	return _socket;
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


bool Server_config::checkCGIbyType(std::string cgi) {
	for(std::vector<Location_config>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		if ((*it).mIsCGI() && (*it).haveCgiPath() && (*it).haveType(cgi))
			return (true);
	}
	return (false);
}

std::string Server_config::getCGIpachByType(std::string cgi) {
	if (!checkCGIbyType(cgi))
		throw Server_config::ServerCGInotFoundException();
	for(std::vector<Location_config>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		if ((*it).mIsCGI() && (*it).haveCgiPath() && (*it).haveType(cgi))
			return ((*it).getCgiPath());
	}
	return ("");
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

void Server_config::setSocket(int sock) {
	if (haveSocket())
		throw Server_config::PortAlraedySetException();																	//ToDo change exception
	else
		_socket = sock;
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

void Server_config::addLocation(std::string location_path, std::string type) {
	//Location_config nLocation(location_path, types);
	//_locations.push_back(Location_config(location_path, types));
	//setReturnCode(0, "");	// Block returnCode

	if (_active_location)
		throw Server_config::AnotherLocationOpenedException();
	// std::cout << "Location_config add: " << location_path << " - " << type << std::endl;
	_locations.push_back(Location_config(location_path, type));
	_active_location = true;
	setReturnCode(0, "");	// Block returnCode
}

int Server_config::acceptNewConnect() {
	struct sockaddr_in address;
	unsigned int addrLen = sizeof(address);
	int sock = accept(_socket, (struct sockaddr *)&address, (socklen_t *)
			&addrLen);
	if (sock < 0) {
		throw Server_config::ServerSocketInitError();
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);
	return sock;
}

void Server_config::initSocket()
{
	if (haveSocket())
		throw Server_config::ServerSocketException();
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		throw Server_config::ServerSocketException();
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr(IP);
	_addr.sin_port = htons(getPort());

	memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));

	int ret = 1;
	int res = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &ret, sizeof(_addr));
	if (res < 0)
		throw Server_config::ServerSocketException();

	res = bind(sock, (struct sockaddr *) &_addr, sizeof(_addr));
	if (res < 0)
	{
		close(sock);
		throw Server_config::ServerSocketException();
	}
	res = listen(sock, 100);
	if (res < 0)
		throw Server_config::ServerSocketException();
	setSocket(sock);
}

void Server_config::checkLastLocation() {
	int n = _locations.size() - 1;
	if (n < 0)
		throw Server_config::SizeLocationsException();
	if (_locations[n].mIsCGI() && _locations[n].haveCgiPath() && !_locations[n].getType().empty())
	{
		// std::cout << "CGI location done" << std::endl; //ToDo enable for debug
		_active_location = false;
	}
	else if (_locations[n].mIsPrefic() && _locations[n].haveRoot())
	{
		// std::cout << "Prefix location done" << std::endl; //ToDo enable for debug
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

const char *Server_config::AnotherLocationOpenedException::what() const throw() {
	return ("EXCEPTION! Another Location Opened in this server...");
};

const char *Server_config::ServerSocketInitError::what() const throw() {
	return ("EXCEPTION! Server Socket Init Error...");
};

const char *Server_config::ServerSocketException::what() const throw() {
	return ("EXCEPTION! Server socket exception...");
};

const char *Server_config::ServerCGInotFoundException::what() const throw() {
	return ("EXCEPTION! Server CGI not Found exception...");
};


//const char *Server_config::CodePageNumberAlraedySetException::what() const throw() {
//	return ("EXCEPTION! Code Page Number Alraedy Set in this server...");
//};

