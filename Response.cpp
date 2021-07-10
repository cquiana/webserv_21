#include "Response.hpp"

Response::Response(/* args */) {

}

Response::~Response() {

}

void Response::setErrors() {
	_errors[200] = "OK";
	_errors[201] = "Created";
	_errors[204] = "No Content";
	_errors[400] = "Bad Request";
	_errors[403] = "Forbidden";
	_errors[404] = "Not Found";
	_errors[405] = "Method Not Allowed";
	_errors[413] = "Payload Too Large";
	_errors[500] = "Internal Server Error";


}

void Response::setDate() {
	char buff[128];
	struct timeval tv;
	struct tm *tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buff, 128, "%a, %d %b %Y %H:%M:%S GMT", tm);
	_date = std::string(buff);
}

void Response::setLastModif(const std::string &str) {
	char buff[128];
	struct stat st;
	struct tm *tm;

	if (stat(str.c_str(), &st) == 0) {
		tm = gmtime(&st.st_mtime);
		strftime(buff, 128, "%a, %d %b %Y %H:%M:%S GMT", tm);
		_lastModif = std::string(buff);
	}
}

void Response::startGenerateResponse() {

}

std::string Response::responseToString() {
	std::ostringstream out;
	out << _protocol << " " << _status << " " << _errors.at(_status) << "\r\n";
	std::map<std::string, std::string>::const_iterator it;
	for (it = _headers.cbegin(); it != _headers.cend(); ++it) {
		out << it->first << ": " << it->second << "\r\n";
	}
	out << "\r\n";
	std::string result = out.str();
	return result + _body;
}

void Response::setHeaders(std::string &key, std::string &value) {
	_headers[ft_tolower(key)] = value;
}

std::string Response::getHeader(std::string &key) const {
	try {
		return _headers.at(ft_tolower(key));
	} catch  (const std::out_of_range &) {
		return "";
	}
}
