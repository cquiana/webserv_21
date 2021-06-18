//
// Created by Candy Quiana on 5/27/21.
//

#include "Webserv.hpp"

void WebServer::initServ() {

	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0) {
		std::cerr << "socket error" << std::endl;
		//TODO: решить, что делать при ошибке
	}

	// залипание порта
	int opt = 1;
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in _addr;
	_addr.sin_family = AF_INET; // for IPv4
	_addr.sin_port = htons(PORT); //  host to network (short)
	_addr.sin_addr.s_addr = inet_addr(IP); // our IP
	// если не получилось с адресом
	if (_addr.sin_addr.s_addr == INADDR_NONE) {
		std::cerr << "inet_addr error" << std::endl;
		//TODO: решить, что делать при ошибке
	}
	if (bind(_sock, ( sockaddr*)&_addr, sizeof(_addr)) == -1) {
		std::cerr << "bind error" << std::endl;
		//TODO: решить, что делать при ошибке
	}
	if (listen(_sock, QLEN) == -1) {
		std::cerr << "listen error" << std::endl;
		//TODO: решить, что делать при ошибке
	}
}

void WebServer::startServ() {
	int new_sock;
	socklen_t sock_len = sizeof(_addr);
	// TODO: получить запрос
	while (1) {
		new_sock = accept(_sock, (sockaddr*)&_addr, &sock_len);
		if (new_sock < 0)
			std::cerr << "accept error!" << std::endl;
		char read_buf[1024] = {0};
		int was_red = recv(new_sock, read_buf, 1024, 0);
//	std::cout << "request: " << read_buf << std::endl;
//		if (was_red < 0)
//			std::cout << "Nothing to read! " << std::endl;
		std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
//	write(new_sock , hello , strlen(hello));
		send(new_sock, hello.c_str(), hello.size(), 0);
//		std::cout << "Waiting new connection!" << std::endl;
		close(new_sock);
	}

}

void WebServer::closeServ() {
	close(_sock);
}

WebServer::WebServer() {}
WebServer::WebServer(WebServer const &x) { (void)x; }
WebServer& WebServer::operator=(WebServer const &x) { (void)x; return *this; }
