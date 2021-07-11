#include "Client.hpp"


Client::Client() { }

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
	if(_request.getCompete())
		_status = READY_TO_SEND;
}

client_status Client::getStatus() {
	return _status;
}

void Client::sendResp() {
	Response resp;
	resp.startGenerateResponse(_request);
	std::string rStr = resp.responseToString();
	int ret = send(getSock(), rStr.c_str(), rStr.length(), 0);
	if (ret <= 0 ) {
		std::cout << "send error\n";
	}
//	send((*it)->getSock(), recp.c_str(), recp.length(), 0);

}


