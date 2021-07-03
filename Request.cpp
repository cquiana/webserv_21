#include "Request.hpp"

#define BUFFER_SIZE 4096

Request::Request(int sock) : _sock(sock), _isComplete(false), _contentLength(0) {
}

Request::~Request() {
}

void Request::parseRequest(std::string request) {
	parseHeaders(request);
	parseBody(request);
}

void Request::parseHeaders(std::string &request) {
	std::string line = cutLine(request, "\r\n");
	_headers["Method"] = cutLine(line, " ");
	_headers["Path"] = cutLine(line, " ");
	_headers["Version"] = cutLine(line, " ");

	line.erase();
	for (line = cutLine(request, "\r\n"); !line.empty(); line = cutLine(request, "\r\n")) {
		std::string key = cutLine(line, ":");
		_headers.insert(std::make_pair(key, line));
	}
}

void Request::parseBody(std::string &request) {
	request.erase(0,2);
	_headers["Body"] = "";
	// TODO: check chuncked
	//
	int len = std::atoi(_headers["Content-lenght"].c_str());
	if (len > request.size()) {
		_isComplete = false;
		return;
	}
	_headers["Body"] = request.substr(0, len);
	_isComplete = true;
}

std::string const &Request::getMethod() {
	return (_headers["Method"]);
}

size_t Request::getContentLength() {
		_contentLength = std::atoi(_headers["Content-Length"].c_str());
		return _contentLength;
}

std::string Request::getHost() {
	std::map<std::string, std::string>::iterator it = _headers.find("Host");
	if (it == _headers.end()) {
		return "";
	} else {
		return it->second;
	}
}
std::string const &Request::getReqBody() {
	return (_headers["Body"]);
}

std::string Request::getAuthType(){
	std::string res;
	res = _headers["Authorization"];
	return res.substr(0, res.find(' '));
}
std::string Request::getConnection(){
	return _headers["Connection"];
}
std::string Request::getUserAgent(){
	return _headers["User-Agent"];
}
std::string Request::getUri(){
	return _headers["Path"];
}
std::string Request::getPath() {
	std::string res;

	res = _headers["Path"];
	size_t query = res.find('?');
	if (query != std::string::npos)
		res.erase(query);
	return res;
}

std::string Request::getQueryString() {
	std::string res;

	res = _headers["Path"];
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

void Request::receive() {
	char buff[BUFFER_SIZE + 1];
	int ret;
	std::cout << "Recv req!\n";
	ret = read(_sock, buff, BUFFER_SIZE);
	if (ret == -1) {
		std::cout << "read error\n";
	} else {
		std::cout << ret << std::endl;
		buff[ret] = '\0';
		_toRead.append(buff);
		bzero(buff, BUFFER_SIZE);
	}
	parseRequest(_toRead);
}
