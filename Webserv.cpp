#include "Webserv.hpp"

WebServer::WebServer() {

}

WebServer::~WebServer() {

}

int WebServer::init(std::string fileName)  {

	_maxSock = 0;
	// parsing
	// vector servers
//	_countServ = ?
// TODO: создать вектор серверов
	FD_ZERO(&_mainFdSet);
	Server tmp(IP, PORT); // ? name
	_serverVect.push_back(tmp);
	// TODO: получить количество серверов из конфига
	_countServ = _serverVect.size();
	for (int i = 0; i < _countServ; ++i) {
		if (_serverVect[i].create(i)) {
			std::cout << "serv init error!\n";
			return 1;
		}
		FD_SET(_serverVect[i].getSock(), &_mainFdSet); // adding sock in set
		if(_serverVect[i].getSock() > _maxSock)
			_maxSock = _serverVect[i].getSock(); // set max # of sock
	}
	return 0;
}

int WebServer::init()  { // file name

	return 0;
}

int WebServer::start() {
	fd_set wrFdSet;
	fd_set tmpSet;

	while (1) {
		tmpSet = _mainFdSet;
		resetWritingSet(&wrFdSet);
		int ret = select(_maxSock + 1, &tmpSet, &wrFdSet, 0, 0);
		if (ret < 0) {
			std::cout << "select error!\n";
		}
		std::vector<Client>::iterator it = _clients.begin();
		for (;  it != _clients.end() ; ++it) {
			if (it->getStatus() == READY_TO_SEND) {
				if (FD_ISSET(it->getSock(), &wrFdSet)) {
					if(!it->sendResp()){
						std::cout << "send resp error!\n";
						if (close(it->getSock()) < 0)
							return 1;
						FD_CLR(it->getSock(), &_mainFdSet);
						it->setStatus(CONNECT_CLOSE);
						_clients.erase(it);
					}
					else if (it->getStatus() == ALL_DATA_SENDET) {
						close(it->getSock());
						it->setStatus(CONNECT_CLOSE);
						_clients.erase(it);
					}
				}
			} else {
				if (close(it->getSock()) < 0)
					return 1;
				FD_CLR(it->getSock(), &_mainFdSet);
				it->setStatus(CONNECT_CLOSE);
				_clients.erase(it);
			}
			break;
		}
		for (int i = 0; i < _countServ; ++i) {
			if (FD_ISSET(_serverVect[i].getSock() , &tmpSet)) {
				int newSock = _serverVect[i].acceptNewConnect();
				if(newSock < 0)
					return 1;
				FD_SET(newSock, &_mainFdSet);
				if (newSock > _maxSock)
					_maxSock = newSock;
				_clients.push_back(Client(newSock, i));
				break;
			}
		}
		it = _clients.begin();
		for (;  it != _clients.end(); ++it) {
			if (it->getStatus() == READY_TO_RECV) {
				if (FD_ISSET(it->getSock(), &_mainFdSet)) {
					if(!it->recvReq()) {
						std::cout << "recv resp error!\n";
						if (close(it->getSock()) < 0)
							return 1;
						FD_CLR(it->getSock(), &_mainFdSet);
						it->setStatus(CONNECT_CLOSE);
						_clients.erase(it);
					}
//					else {
//						it->setStatus(ALL_DATA_SENDET);
//						FD_CLR(it->getSock(), &_mainFdSet);
//						_clients.erase(it);
//					}
				}
			} else {
				if (close(it->getSock()) < 0)
					return 1;
				FD_CLR(it->getSock(), &_mainFdSet);
				it->setStatus(CONNECT_CLOSE);
				_clients.erase(it);
			}
			break;
		}
	}

	return 0;
}

void WebServer::stop() {

	if (!_clients.empty()) {
		for (std::vector<Client>::iterator it = _clients.begin();  it != _clients.end() ; ++it) {
			close(it->getSock());
		}
	}
	for (int i = 0; i < _serverVect.size(); ++i) {
		close(_serverVect[i].getSock());
	}
}

void WebServer::resetWritingSet(fd_set *wrFdSet) {
	FD_ZERO(wrFdSet);
	std::vector<Client>::iterator it = _clients.begin();
	for (;  it != _clients.end() ; ++it) {
		if (it->getStatus() == READY_TO_SEND)
			FD_SET(it->getSock(), wrFdSet);
	}
}



//WebServer &WebServer::operator=(const WebServer &rhs) {
//	return <#initializer#>;
//}
