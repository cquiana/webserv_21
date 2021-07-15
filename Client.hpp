#ifndef CLIENT_HPP
# define CLIENT_HPP
#include "Request.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include <unistd.h>


class Request;
class Response;

enum client_status {
	READY_TO_RECV,
	READY_TO_SEND,
	ALL_DATA_SENDET,
	CONNECT_CLOSE
};

class Client
{
private:
	Server _serv;
	int _sock;
//	int _idxSrv;
	Request _request;
	std::string _requestMsg;
	std::string _responseMsg;
	client_status _status;



public:
	Client();
	Client(Server &serv, int sock);
	Client(int sock);
	Client(int sock, int idxSrv);
	~Client();

	void setStatus(client_status st);
	void setRequest(Request &req);

	int getSock();
	bool recvReq();
	bool sendResp();
	int getIndexOfServ();
	client_status getStatus();

//	Response startGenerateResponse(Request request);
};
#endif




