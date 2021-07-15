#include "Webserv.hpp"

WebServer::WebServer(Http_config* http_config) {
	_http_config = http_config;
	FD_ZERO(&_mainFdSet);
	_maxSock = 0;
	for(std::vector<Server_config>::iterator it = _http_config->_servers.begin(); it != _http_config->_servers.end();
	it++)
	{
		FD_SET((*it).getSocket(), &_mainFdSet);
		if((*it).getSocket() > _maxSock)
			_maxSock = (*it).getSocket(); // set max # of sock
	}
}

WebServer::~WebServer() {}

//int WebServer::init(std::string aaa)  {
//
//	_maxSock = 0;
//	// parsing
//	// vector servers
// //	_countServ = ?
//
//	Server tmp("getName", 123); // ? name
//	_serverVect.push_back(tmp);
//
//	_countServ = _serverVect.size();
//	for (int i = 0; i < _countServ; ++i) {
//		if (_serverVect[i].create(i)) {
//			std::cout << "serv init error!\n";
//			return 1;
//		}
//		FD_SET(_serverVect[i].getSock(), &_mainFdSet); // adding sock in set
//		if(_serverVect[i].getSock() > _maxSock)
//			_maxSock = _serverVect[i].getSock(); // set max # of sock
//	}
//	return 0;
//}

//int WebServer::init()  { // file name
//
//	return 0;
//}

int WebServer::loop() {
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

		for(std::vector<Server_config>::iterator it2 = _http_config->_servers.begin(); it2 != _http_config->_servers.end(); it2++)
		{
			if (FD_ISSET((*it2).getSocket() , &tmpSet)) {
				int newSock = (*it2).acceptNewConnect();
				if(newSock < 0)
					return 1;
				FD_SET(newSock, &_mainFdSet);
				if (newSock > _maxSock)
					_maxSock = newSock;
//				_clients.push_back(Client(newSock, i));
				_clients.push_back(Client(newSock));
				break;
			}
		}


//		for (int i = 0; i < _countServ; ++i) {
//			if (FD_ISSET(_serverVect[i].getSock() , &tmpSet)) {
//				int newSock = _serverVect[i].acceptNewConnect();
//				if(newSock < 0)
//					return 1;
//				FD_SET(newSock, &_mainFdSet);
//				if (newSock > _maxSock)
//					_maxSock = newSock;
//				_clients.push_back(Client(newSock, i));
//				break;
//			}
//		}
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

	for(std::vector<Server_config>::iterator it3 = _http_config->_servers.begin(); it3 != _http_config->_servers.end(); it3++)
	{
		close((*it3).getSocket());
	}

//	for (int i = 0; i < _serverVect.size(); ++i) {
//		close(_serverVect[i].getSock());
//	}
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
