#pragma once
#include "Request.hpp"
#include "Server.hpp"
#include <unistd.h>


// class Request;

enum client_status {
	READY_TO_RECV,
	READY_TO_SEND,
	SENDING,
	CONNECT_CLOSE
};

class Client
{
private:
	Server _serv;
	int _sock;
	Request _request;
	std::string _reqMsg;
	client_status _status;

	Client();

public:
	Client(Server &serv, int sock);
	Client(int sock);
	~Client();

	int getSock();
	void recvReq();
};




