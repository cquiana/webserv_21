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

Server::Server(std::string ip, size_t port) : _ip(ip), _port(port){

}
Server::Server(std::string ip, size_t port, std::string name) {

}

int Server::create(int i) {
	_id = 1;
	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0) {
		std::cout << "socket error!\n";
		return 1;
	}
	// from config
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr(_ip.c_str());
	_addr.sin_port = htons(_port);

	memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));

	int ret = 1;
	int res = setsockopt(_sock, SOL_SOCKET, SO_REUSEPORT, &ret, sizeof(_addr));
	if (res < 0) {
		std::cout << "sotsock error!\n";
		return  1;
	}

	res = bind(_sock, (struct sockaddr *)&_addr, sizeof(_addr));
	if (res < 0) {
		close(_sock);
		std::cout << "bind error!\n";
		return 1;
	}
	res = listen(_sock, 100);
	if (res < 0) {
		std::cout << "listen error!\n";
		return 1;
	}
	return 0;
}

int Server::acceptNewConnect() {
	struct sockaddr_in address;
	unsigned int addrLen = sizeof(address);
	int sock = accept(_sock, (struct sockaddr *)&address, (socklen_t *)
			&addrLen);
	if (sock < 0) {
		std::cout << "accept error!\n";
		return -1;
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);
	return sock;
}


