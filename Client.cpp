#include "Client.hpp"
#define BUFFER_SIZE 10000;


Client::Client() { }

Client::Client(int sock) : _sock(sock), _request(sock), _status(READY_TO_RECV) {

}

Client::Client(int sock, int idxSrv) : _sock(sock), _idxSrv(idxSrv), _status(READY_TO_RECV) {

}

Client::Client(Server &serv, int sock) : _serv(serv), _sock(sock), _request(sock){

}

Client::~Client() {
//	close(_sock);
}

int Client::getSock() {
	return _sock;
}
bool Client::recvReq() {
	Request request(_sock);
	if (_status == READY_TO_RECV) {
		if (!request.receive())
			return false;
	}
	if (request.getCompete()) {
		setRequest(request);
	}
	_status = READY_TO_SEND;
	return true;
}

client_status Client::getStatus() {
	return _status;
}

bool Client::sendResp() {

	Response response(200);
	response.startGenerateResponse(_request);
//	response.setDefaultHeader();
	std::string res = response.responseToString();

	int ret = send(_sock, res.c_str(), res.length(), 0);
	if (ret < 0) {
		std::cout << "send error\n";
		return false;
	}
	if (ret == response.getContetntLength())
		_status = ALL_DATA_SENDET;
//	_responseMsg.erase(0, ret); // clear responce

	return true;
}


void Client::setStatus(client_status st) {
	_status = st;
}

int Client::getIndexOfServ() {
	return _idxSrv;
}

void Client::setRequest(Request &req){
	_request = req;
}
