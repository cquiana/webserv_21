#include "Client.hpp"

// Client::Client() { }

Client::Client(int sock) : _sock(sock), _request(sock), _reqMsg(""), _status(READY_TO_RECV) {

}

Client::Client(Server &serv, int sock) : _serv(serv), _sock(sock), _request(sock), _reqMsg(""){

}

Client::~Client() {
	close(_sock);
}

int Client::getSock() {
	return _sock;
}
void Client::recvReq() {
	if (_status == READY_TO_RECV)
		_request.receive();
}
