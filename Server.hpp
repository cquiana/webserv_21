#pragma once
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
// #include "Webserv.hpp"

class Server
{
private:
	std::string _host;
	std::string _name;
	std::string _root;
	size_t _port; // or ports

	int _sock;
	struct sockaddr_in _addr;
	// std::vector<Location> _location;

public:
	Server();
	Server(const Server &copy);
	~Server();
	Server &operator=(const Server &copy);

	void setHost(const std::string &host);
	void setName(const std::string &name);
	void setRoot(const std::string &root);
	void setPort(const size_t port);
	void setSock(const int sock);
	void setSock_Addr(const sockaddr_in &addr);

	std::string getHost() const;
	std::string getName() const;
	std::string getRoot() const;
	size_t getPort() const;
	int getSock() const;
	sockaddr_in getSock_Addr();

};


