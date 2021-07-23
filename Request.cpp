#include "Request.hpp"

#define BUFFER_SIZE 100000

Request::Request(int sock) : _contentLength (0), _isComplete(false),
							_correctBody(true), _sock(sock) {
}

Request::Request() {
}

Request::~Request() {
}

void Request::setLocationPath(){
	std::string location = getUri();
	size_t pos = location.find_last_of('/');
	_location = location.substr(0, pos);
}

void Request::setHeader(const std::string &key, const std::string &value) {
	_headers[key] = value;
}

void Request::parseRequest(std::string request) {
	parseHeaders(request);
	setHeader("query-string", _queryString);
	setContentLength();
	setQueryString();
	parseBody(request);
}

void Request::parseHeaders(std::string &request) {
	std::string line = cutLine(request, "\r\n");
	_headers["method"] = cutLine(line, " ");
	_headers["path"] = cutLine(line, " ");
	_headers["version"] = cutLine(line, " ");
	setLocationPath();
	line.erase();
	for (line = cutLine(request, "\r\n"); !line.empty(); line = cutLine(request, "\r\n")) {
		std::string key = ft_skip_space(cutLine(line, ": "));
		_headers.insert(std::make_pair(ft_tolower(key), line));
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
//		int len = std::atoi(_headers["content-length"].c_str());
		int len = stringToNumber(_headers["content-length"]);
		if (static_cast<size_t>(len) > request.size()) {
			_correctBody = false;
			return;
		}
		_headers["body"] = request.substr(0, len);
		_isComplete = true;
	}
}

const std::string & Request::getMethod() const
{
	return (_headers.at("method"));
}


int Request::getContentLength() {
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
std::string  &Request::getReqBody() {//ToDo check key in map
	return (_headers["body"]);
}

std::string Request::getAuthType(){ //ToDo check key in map
	std::string res;
	res = _headers["authorization"];
	return res.substr(0, res.find(' '));
}
std::string Request::getConnection(){//ToDo check key in map
	return _headers["connection"];
}
std::string Request::getUserAgent(){//ToDo check key in map
	return _headers["user-agent"];
}
std::string Request::getUri(){//ToDo check key in map
	return _headers["path"];
}
std::string Request::getPath() const//ToDo check key in map
{
	std::string res;

	res = _headers.at("path");
	size_t query = res.find('?');
	if (query != std::string::npos)
		res.erase(query);
	return res;
}

std::string Request::getQueryString() {
	return _queryString;

}

bool Request::getCompete() const {
	return _isComplete;
}

// bool Request::checkHeader() {
// 	return (_toRead.find("\r\n\r\n") != std::string::npos);
// }

bool Request::receive() {
	char buff[BUFFER_SIZE + 1]= {};
	int ret;
	ret = recv(_sock, buff, BUFFER_SIZE, 0);
	if (ret < 0) {
//		throw Request::ReadErrorException();

		std::cout << "read error\n";
		return false;

	} else {
//		std::cout << ret << std::endl;
		buff[ret] = '\0';
		_toRead.append(buff);
		bzero(buff, BUFFER_SIZE);
	}
	parseRequest(_toRead);
	return true;
}

const std::string Request::getHttpVers() const
{
	std::string res;

	res = _headers.at("version");
	return res;
}

std::string Request::getLocatinPath() const
{
	return _location;
}

void Request::eraseRequest(){
	_headers.clear();
	_uri.clear();
	_queryString.clear();
	_reqBody.clear();
	_location.clear();
	_toRead.clear();
	_contentLength = 0;
	_isComplete = false;
	_correctBody = false;
	_sock = 0;

}

//std::string  Request::getReqBody() {
//	return _reqBody;
//}
void Request::setQueryString() {
	std::string res;

	_queryString = "";
	res = _headers["path"];

//	std::cout << "##### " << res << " @@@@@\n";

	size_t query = res.find('?');
	if (query == std::string::npos)
		_queryString =  "";
	else
		_queryString += res.substr(query + 1);
//	std::cout << "##### " << _queryString << " @@@@@\n";
}

void Request::setContentLength() {
//	_errors.count(getErrorCode())
	if (!_headers.count("content-length"))
		_contentLength = 0;
	else
		_contentLength = stringToNumber(_headers.at("content-length"));
}


const char *Request::ReadErrorException::what() const throw() {
	return ("EXCEPTION! Read from socket error...");
};


