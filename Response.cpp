#include "Response.hpp"

Response::Response(/* args */) {

}

Response::Response(int code) : _code(code) {
	setErrors();
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
	_errors[505] = "HTTP version not supported";

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

//void Response::startGenerateResponse() {
//
//}

std::string Response::responseToString() {
	std::ostringstream out;
	out << "HTTP/1.1 " << _code << " " << _errors.at(_code) << "\r\n";
	std::map<std::string, std::string>::const_iterator it;
	for (it = _headers.cbegin(); it != _headers.cend(); ++it) {
		out << it->first << ": " << it->second << "\r\n";
	}
	out << "\r\n";
	std::string result = out.str();
	return result += _body;
}

void Response::setHeaders(const std::string &key, const std::string &value) {
	_headers[ft_tolower(key)] = value;
}

std::string Response::getHeader(const std::string &key) const {
	try {
		return _headers.at(ft_tolower(key));
	} catch  (const std::out_of_range &) {
		return "";
	}
}

Response Response::startGenerateResponse(Request &request) {


	if (request.getCompete() == false)
		setErrorPage(400);
	if (request.getHttpVers() != "HTTP/1.1")
		setErrorPage(505);
	std::cout << request.getMethod() << std::endl;
//	if (isDirectory("/Users/cquiana/CLionProjects/webserv_21/index.html"))
//		std::cout << "/Users/cquiana/CLionProjects/webserv_21\n";
	if (request.getMethod() == "GET") {
		Response  getResp = generateGET(request);
		return getResp;
	}
	else if (request.getMethod() == "POST") {
//		Response  getResp = generateGET(request);
//		return getResp;
	}
	else if (request.getMethod() == "DELETE") {
//		Response  getResp = generateGET(request);
//		return getResp;
	}
	else {
//		Response  getResp = generateGET(request);
//		return getResp;
	}

Response res(200);
	return (res);
	/*в целом шаги такие:
- разбираешься с путем до файла (в зависимости от конфига)
- проверяешь есть ли он, права доступа, папка ли это (мы еще тут же проверяли, сgi ли это, и, если да, то запускали и дальше уже)
- если права есть, не папка, то читаем из него в буфер и отправляем
- если папка и включен автоиндекс, то выводили страничку-перечисление содержимого, как в nginx autoindex
- если ошибки, то формировали хедер с ошибкой и отправляли
- если cgi, то когда cgi будет возвращать что-то в ответ, отправляем это в ответе*/

}

Response Response::generateGET(Request &request) {

	Response resp(200);

	// get path from root

	// path + request path

	std::string rootPath = "/Users/cquiana/CLionProjects/webserv_21";
	int autoindex = 1;

	std::string fullPath(rootPath + request.getPath());

	if (isDirectory(rootPath)) {
		// check autoindex
		if (autoindex == 1) {
			// generate view
		}
	}

	//TODO: check CGI
	if (checkCGI(request))
		resp = generateCGI(request);
	else {
		struct stat st;
		std::stringstream  buff;
		fullPath += "index.html";
		std::ifstream file(fullPath);
		if (!file.is_open()) {
			std::cerr << "Open file error\n";
		}
		buff << file.rdbuf();
		file.close();
		std::string body = buff.str();
		setBody(body);
//		std::cout << body.length() << std::endl;
		setContentLength(body.length());
	}
	setDefaultHeader();
	return  resp;
}



void Response::setErrorPage(int code) {

}

void Response::errorPageFromFile(const std::string &path) {
	std::stringstream  buff;
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Open file error\n";
	}
	buff << file.rdbuf();
	file.close();
	std::string body = buff.str();
	setBody(body);

	setHeaders("Content-Type", "test/html");
}

void Response::setBody(std::string &body) {
	_body = body;
}

bool Response::isAutoIndex() {
	return false;
}

void Response::setDefaultHeader() {
//	setHeaders("");
	setDate();
	setHeaders("Content-Type", "test/html");
//	setHeaders("Connection", "close");
	setHeaders("Date", _date);
	if (getHeader("Content-Length").empty())
		setHeaders("Content-Length", numberToString(_body.length()));

}

std::string Response::getDate() {
	return _date;
}

void Response::setContentLength(size_t len) {
	_lenght = len;
}

size_t Response::getContetntLength() {
	return _lenght;
}

Response Response::generateCGI(const Request &request) {
	Response respCGI(200);
	return respCGI;
}

bool Response::checkCGI(const Request &request) {
		std::string tmp = _request.getPath();
		size_t dot = tmp.find_last_of('.');
		std::string ext = tmp.substr(dot + 1);
		return (ext == "py" || ext == "js"); // или js
}




