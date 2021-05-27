//
// Created by Candy Quiana on 5/27/21.
//

#include "Webserv.hpp"

WebServer & WebServer::Instance() {
	static WebServer theSingleInstance;
	return theSingleInstance;
}

void WebServer::setup() {

	ls = socket(AF_INET, SOCK_STREAM, 0);
	if (ls == -1) {
		std::cerr << "WebSocket.setup(): socket error" << std::endl;
		//TODO: решить, что делать при ошибке
	}

	/*
	 * Для избежания залипания порта.
	 * TODO: Решить, стоит ли оставлять в готовой программе
	 */
	int opt = 1;
	setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	if (addr.sin_addr.s_addr == INADDR_NONE) {
		std::cerr << "WebSocket.setup(): inet_addr error" << std::endl;
		//TODO: решить, что делать при ошибке
	}
	if (bind(ls, ( sockaddr*)&addr, sizeof(addr)) == -1) {
		std::cerr << "WebSocket.setup(): bind error" << std::endl;
		//TODO: решить, что делать при ошибке
	}
	if (listen(ls, QLEN) == -1) {
		std::cerr << "WebSocket.setup(): listen error" << std::endl;
		//TODO: решить, что делать при ошибке
	}
}

void WebServer::run() {
	//TODO: Реализовать функционал.
	int cls;
	socklen_t slen = sizeof(addr);
	cls = accept(ls, (sockaddr*)&addr, &slen);
	write(cls, "HTTP/1.1 200 OK\n", 16);
	close(cls);
}

void WebServer::finish() {
	close(ls);
}

WebServer::WebServer() {}
WebServer::WebServer(WebServer const &x) { (void)x; }
WebServer& WebServer::operator=(WebServer const &x) { (void)x; return *this; }
