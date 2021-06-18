//
// Created by Candy Quiana on 5/27/21.
//


#include "Webserv.hpp"

int main() {

	WebServer server;

	server.initServ();
	server.startServ();
	server.closeServ();

	return 0;
}
