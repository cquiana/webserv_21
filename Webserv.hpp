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
#include "Server.hpp"
#include "Http_config.hpp"


#define PORT 8888
#define IP "127.0.0.1"

class Client;
class WebServer {
public:
	WebServer();
	WebServer(WebServer const & src);  				//copy
	~WebServer();								//destructor
	WebServer & operator=(WebServer const & rhs);		//overload operator =

	int init( std::string fileName );
	int init();
	int start();
	void stop();


	void setReadStatus(long socket);
	void setWritingSetFD(fd_set *wrFD);

	std::map<std::string, std::string> getMap();
	fd_set getMasterSet();
	std::vector<Server> getServerList();

	void resetWritingSet(fd_set *wrFdSet);


private:

	int _maxSock;
	fd_set _mainFdSet;
	std::vector<Server> _serverVect;
	std::vector<Client> _clients;
	int _countServ;
	Http_config _config;

};

