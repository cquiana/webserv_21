//
// Created by Candy Quiana on 5/27/21.
//


#include "Webserv.hpp"

int main() {

	WebServer server;


	server.initServ();
	while (1) {
		server.startServ();
		server.waitConnect();
		server.createSock();
		std::cout << "Main!\n";
		sleep(15);
	}
	server.closeServ();


	return 0;
}
