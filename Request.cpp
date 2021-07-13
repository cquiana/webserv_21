#include "Request.hpp"

#define BUFFER_SIZE 4096

Request::Request(int sock) : _sock(sock), _isComplete(false),
							_correctBody(true), _contentLength (0) {
}

Request::Request() {
}

Request::~Request() {
}

void Request::parseRequest(std::string request) {
	parseHeaders(request);
	parseBody(request);
}

void Request::parseHeaders(std::string &request) {
	std::string line = cutLine(request, "\r\n");
	_headers["method"] = cutLine(line, " ");
	_headers["path"] = cutLine(line, " ");
	_headers["version"] = cutLine(line, " ");

	line.erase();
	for (line = cutLine(request, "\r\n"); !line.empty(); line = cutLine(request, "\r\n")) {
		std::string key = ft_skip_space(cutLine(line, ":"));
		_headers.insert(std::make_pair(ft_tolower(key), ft_skip_space(line)));
	}
}

void Request::parseBody(std::string &request) {
	request.erase(0,2);
	_headers["body"] = "";
	if (_headers["transfer-encoding"] == "chunked") {
		std::cout << "chunked\n";
		long size;
		while(!request.empty()) {
			size = checkSize(cutLine(request, "\r\n").c_str(), 16);
			_headers["body"] += request.substr(0, size);
			if(size < 0 || size > (long) request.size() + 2)
				_correctBody = false;
			request.erase(0, size + 2);
		}
	} else { //if (_headers["Content-length"])
		int len = std::atoi(_headers["content-length"].c_str());
		if (len > request.size()) {
			_correctBody = false;
			return;
		}
		_headers["body"] = request.substr(0, len);
		_isComplete = true;
	}

}

const std::string  &Request::getMethod() {
	return (_headers["method"]);
}

size_t Request::getContentLength() {
		_contentLength = std::atoi(_headers["content-length"].c_str());
		return _contentLength;
}

std::string Request::getHost()  {
	std::map<std::string, std::string>::iterator it = _headers.find("host");
	if (it == _headers.end()) {
		return "";
	} else {
		return it->second;
	}
}
std::string  &Request::getReqBody() {
	return (_headers["body"]);
}

std::string Request::getAuthType(){
	std::string res;
	res = _headers["authorization"];
	return res.substr(0, res.find(' '));
}
std::string Request::getConnection(){
	return _headers["connection"];
}
std::string Request::getUserAgent(){
	return _headers["user-agent"];
}
std::string Request::getUri(){
	return _headers["path"];
}
std::string Request::getPath() {
	std::string res;

	res = _headers["path"];
	size_t query = res.find('?');
	if (query != std::string::npos)
		res.erase(query);
	return res;
}

std::string Request::getQueryString() {
	std::string res;

	res = _headers["path"];
	size_t query = res.find('?');
	if (query == std::string::npos)
		return "";
	else
		return res.substr(query + 1);
}

bool Request::getCompete() {
	return _isComplete;
}

// bool Request::checkHeader() {
// 	return (_toRead.find("\r\n\r\n") != std::string::npos);
// }

bool Request::receive() {
	char buff[BUFFER_SIZE + 1]= {};
	int ret;
	ret = read(_sock, buff, BUFFER_SIZE);
	if (ret < 0) {
		std::cout << "read error\n";
		return false;
	} else {
		std::cout << ret << std::endl;
		buff[ret] = '\0';
		_toRead.append(buff);
		bzero(buff, BUFFER_SIZE);
	}
	parseRequest(_toRead);
	return true;
//	std::cout << ret << std::endl;

//	std::cout << getMethod() << std::endl;
}

std::string Request::getHttpVers()
{
	std::string res;

	res = _headers["version"];
	return res;
}
