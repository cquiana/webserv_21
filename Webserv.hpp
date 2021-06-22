//
// Created by Candy Quiana on 5/27/21.
//

#ifndef WEBSERV__HPP
#define WEBSERV__HPP

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <map>
#include <vector>


#define PORT 1234
#define QLEN 10
#define IP "127.0.0.1"

class WebServer {
public:
	void initServ();
	void startServ();
	void closeServ();
	WebServer();
//	std::string _res;

	std::string createResponse();

private:

	int _sock; // listening socket;
	struct sockaddr_in _addr; // _sock addr


//	WebServer();
	WebServer(WebServer const &);
	WebServer& operator=(WebServer const &);
};

#endif //WEBSERV_HPP
