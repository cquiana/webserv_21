#include "Client.hpp"
#define BUFFER_SIZE 10000;


Client::Client(int sock, Http_config *http_config) : _sock(sock), _request(sock), _status(READY_TO_RECV),
_http_config(http_config) {

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

	Response response(200, _http_config);
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

void Client::setRequest(Request &req){
	_request = req;
}
