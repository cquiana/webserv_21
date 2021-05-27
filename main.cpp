//
// Created by Candy Quiana on 5/27/21.
//


#include "WebServer.hpp"

int main() {

	WebServer &server = WebServer::Instance();

	server.setup();
	server.run();
	server.finish();

	return 0;
}
