#pragma once

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
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include "utils.hpp"
#include "Client.hpp"
// #include "Server.hpp"


#define PORT 1234
#define QLEN 10
#define IP "127.0.0.1"

class Client;
class WebServer {
public:
	void initServ();

	void startServ();
	void closeServ();
	WebServer();
//	std::string _res;

	std::string createResponse();
	int getMaxFd();
	void setMaxFd();
	void createNewConnect();
	void waitConnect();
	void createSock();
	// void reloadFd();
	const std::vector<Client*> &getClients();

private:

	std::vector<Server> _servers;
	std::vector<Client*> _clients;
	size_t _servMaxCount;
	int _sock;
	int _maxFd;
	struct sockaddr_in _addr;
	fd_set _rFd;
	fd_set _wFd;



	// WebServer();
	WebServer(WebServer const &);
	WebServer& operator=(WebServer const &);
};

