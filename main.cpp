#include "Webserv.hpp"

WebServer server;

void sigHandler(int num) {
	(void)num;
	std::cout << "stoping webserv\n";
	server.stop();
	exit(1);
}

void catchSig(void){
	signal(SIGINT, sigHandler);
	signal(SIGQUIT, sigHandler);
}

int main() {

	std::string conf = "example.conf";
//	WebServer server;
	try {
//		if (server.init()) // conf file
		if (server.init(conf))
			return 1;
		// signal
		catchSig();
		if (server.start())
			return  1;
	} catch (const std::exception& e) {
		server.stop();
		return 1;
	}
	return 0;
}
