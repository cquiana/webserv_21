//
// Created by Candy Quiana on 5/27/21.
//

#include "Webserv.hpp"

WebServer::WebServer(/* std::vector<Server> &servers*/) {
	// struct sockaddr_in _addr;
	_addr.sin_family = AF_INET; // for IPv4
	_addr.sin_port = htons(PORT); //  host to network (short)
	_addr.sin_addr.s_addr = inet_addr(IP); // our IP
	FD_ZERO(&_wFd);
	FD_ZERO(&_rFd);
	_maxFd = -1;
}
WebServer::WebServer(WebServer const &x) { (void)x; }

void WebServer::initServ() {

	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0) {
		std::cerr << "socket error" << std::endl;
	//TODO: решить, что делать при ошибке
	}
	// fcntl(_sock, F_SETFL, O_NONBLOCK);
	// залипание порта
	int opt = 1;
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(_sock, (struct sockaddr*)&_addr, sizeof(sockaddr)) == -1) {
		std::cerr << "bind error" << std::endl;
		//TODO: решить, что делать при ошибке
	}
	if (listen(_sock, QLEN) == -1) {
		std::cerr << "listen error" << std::endl;
		//TODO: решить, что делать при ошибке
	}
	setMaxFd();
	// fcntl(_sock, F_SETFL, O_NONBLOCK);
}

void WebServer::startServ() {
	// int new_sock = 0;
	// char read_buf[1024] = {0};


	// TODO: решить в какой форме хоранить дескрипторы set, list, vector

	// TODO: получить запрос
	FD_ZERO(&_rFd);
	FD_ZERO(&_wFd);
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		FD_SET((*it).getSock(), &_rFd);
		FD_SET((*it).getSock(), &_wFd);
	}
	//  cycle servers
	FD_SET(_sock, &_rFd);

	setMaxFd();
	// std::cout << _maxFd << std::endl;
	struct timeval timeout = {10, 0};
	int mx = select(_maxFd + 1, &_rFd, &_wFd, NULL, &timeout);
	if (mx < 0)
		std::cerr << "select error!" << std::endl;

	// fcntl(_sock, F_SETFL, O_NONBLOCK);
	std::cout << "Webserv!\n";
}

void WebServer::waitConnect() {
			//  cycle servers
	// std::cout << _sock << std::endl;
	if (FD_ISSET(_sock, &_rFd)) {
	// 	std::cout << "Create Connection!\n";
		createNewConnect(); /*createNewConnect(i); */
	}
}

void WebServer::createNewConnect() {
	sockaddr_in addrClient;
	int sock_len = sizeof(_addr);
	int new_connect = accept(_sock, (sockaddr*) &_addr, (socklen_t*)&sock_len);
	 std::cout << "newsock = " << new_connect << std::endl;
	if (new_connect < 0)
		std::cerr << "accept error!" << std::endl;
	Client new_client(new_connect);
	_clients.push_back(new_client);
	// std::cout << _clients.size() << std::endl;
	fcntl(new_connect, F_SETFL, O_NONBLOCK);
	// std::cout << "Accept!\n";
	FD_SET(new_connect, &_rFd);
	FD_SET(new_connect, &_wFd);
}

void WebServer::createSock() {
	std::vector<Client>::iterator it;
	// std::cout << _clients.size() << std::endl;s
	for (it = _clients.begin(); it != _clients.end(); ++it) {
		// добавить проверку статуса
		std::cout << "Client sock = " << (*it).getSock() << std::endl;
		if (FD_ISSET((*it).getSock(), &_rFd)) {
			char buff[4096 + 1];
			int ret = read((*it).getSock(), buff, 4096);
			std::cout << ret << std::endl;
			if (ret == -1) {
				std::cout << "read error\n";
			} else {
				std::cout << ret << std::endl;
				buff[ret] = '\0';
				// _toRead.append(buff);
				bzero(buff, 4096);
			}
			// (*it).recvReq();
			std::cout << "recv\n";
		}
		else if (FD_ISSET((*it).getSock(), &_wFd)) {
			// (*it).sendResp();
			std::cout << "send\n";
		}
	}
}

std::string WebServer::createResponse() {
	std::string res;
	std::string tmp;
	std::ifstream is("./html/main.html");
	if (is.is_open()) {
		while (getline(is, tmp)) {
			res.append(tmp);
		}
	}
	// std::cout << "you are here\n";
	is.close();
	// std::cout << res;
	return res;
}

void WebServer::closeServ() {
	close(_sock);
}

// WebServer::WebServer() {}
// WebServer::WebServer(WebServer const &x) { (void)x; }
WebServer& WebServer::operator=(WebServer const &x) { (void)x; return *this; }

const std::vector<Client> &WebServer::getClients(){
	return _clients;
}
int WebServer::getMaxFd() {
	return _maxFd;
}

void WebServer::setMaxFd() {
	_maxFd = _sock;
	for (size_t i = 0; i < getClients().size(); ++i) {
		_maxFd = std::max(_clients[i].getSock(), _maxFd);
	}

	// for (size_t i = 0; i < getClients().size(); ++i) {
	// 	_maxFd = std::max(_servers[i].getSock(), _maxFd);
	// }
}
