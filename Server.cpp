#include "Server.hpp"

Server::Server(/* args */){
}

Server::~Server() {
}

Server::Server(const Server &copy){
	*this = copy;
}

Server& Server::operator=(const Server &copy) {
	_host = copy._host;
	_name = copy._name;
	_root = copy._root;
	_port = copy._port;
	_sock = copy._sock;
	_addr = copy._addr;
	return (*this);
}

void Server::setHost(const std::string &host) {
	_host = host;
}

void Server::setName(const std::string &name) {
	_name = name;
}

void Server::setRoot(const std::string &root) {
	_root = root;
	if (_root.back() != '/')
		_root += '/';
}

void Server::setPort(const size_t port) {
	_port = port;
}

void Server::setSock(const int sock) {
	_sock = sock;
}

void Server::setSock_Addr(const sockaddr_in &addr) {
	_addr = addr;
}

std::string Server::getHost() const {
	return _host;
}
std::string Server::getName() const{
	return _name;
}
std::string Server::getRoot() const {
	return _root;
}
size_t Server::getPort() const {
	return _port;
}
int Server::getSock() const{
	return _sock;
}
sockaddr_in Server::getSock_Addr(){
	return _addr;
}
