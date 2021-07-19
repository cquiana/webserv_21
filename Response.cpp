#include "Response.hpp"

//Response::Response(int code, Http_config* http_config) : _code(code), _http_config(http_config) {
//	setErrors();
//}

Response::Response(int code, Server_config &server_config, const Request &request) : _code(code),
_server_config(server_config), _request(request) {
	setErrors();
}


Response::~Response() {

}

void Response::setErrors() {
	_errors[200] = "OK";
	_errors[201] = "Created";
	_errors[204] = "No Content";
	_errors[300] = "Multiple Choices";
//	_errors[301] = "Moved Permanently";
//	_errors[302] = "Found";
//	_errors[303] = "See Other";
//	_errors[304] = "Not Modified";
//	_errors[305] = "Use Proxy";
//	_errors[307] = "Temporary Redirect";
//	_errors[308] = "Permanent Redirect";
	_errors[400] = "Bad Request";
	_errors[403] = "Forbidden";
	_errors[404] = "Not Found";
	_errors[405] = "Method Not Allowed";
	_errors[413] = "Payload Too Large";
	_errors[500] = "Internal Server Error";
	_errors[501] = "Not Implemented";
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
	_headers[key] = value;
}

std::string Response::getHeader(const std::string &key) const {
	try {
		return _headers.at(key);
	} catch  (const std::out_of_range &) {
		return "";
	}
}

Response Response::startGenerateResponse() {

	setFullPath();
	if (!_request.getCompete())
		setErrorCode(400);
	if (_request.getHttpVers() != "HTTP/1.1")
		setErrorCode(505);
	if (_request.getMethod() == "GET")
		generateGET();
	else if (_request.getMethod() == "POST") {
//		Response  getResp = generateGET(request);
//curl -X POST -F 'image=@/Users/cquiana/Desktop/img.png' http://127.0.0.1:60080/images
//		return getResp;
//		getResp = generatePOST(request);
	}
	else if (_request.getMethod() == "DELETE") {
		methodDELETE();
	}
	else {
//		Response  getResp = generateGET(request);
//		return getResp;
	}
	finishGenerateResponse();
	setDefaultHeader();
	return *this;
}

void Response::finishGenerateResponse() {
	if (getErrorCode() >= 400 && _body.empty()) {
//		if (_errors.count(_code)) {
//			try {
//				errorPageFromFile(_errors.at(_code));
//			} catch (std::runtime_error &e) {
//				std::cerr << e.what() << std::endl;
//				errorPageGenerator(getErrorCode());
//			}
//		}
//		else
			errorPageGenerator(getErrorCode());
	}

}

bool Response::methodDELETE() {

	for (std::vector<Location_config>::iterator it = _server_config._locations.begin(); it !=  _server_config
			._locations.end(); ++it) {
		if ((*it).getLocationPrefix() == _request.getLocatinPath()) {
			if ((*it).getMethods() < 1) {
				setErrorCode(405);
				return false;
			}
		}
	}
	std::string fullPath(_server_config.getRoot() + _request.getPath());
	int ret = std::remove(fullPath.c_str());
//	int ret = unlink(fullPath.c_str());
	if (ret < 0)
		setErrorCode(403);
	else
		setErrorCode(200);
	return true;
}

bool Response::generateGET() {

//	std::string
	for (std::vector<Location_config>::iterator it = _server_config._locations.begin(); it !=  _server_config
	._locations.end(); ++it) {
		if ((*it).getLocationPrefix() == _request.getLocatinPath()) {
			if ((*it).getMethods() < 1) {
				setErrorCode(405);
				return false;
			}
		}
	}

	std::string absPath;
//	std::string fullPath(_server_config.getRoot() + _request.getPath());
//	if (isDirectory(fullPath)) {
//		if (fullPath.back() != '/')
//			fullPath += "/";
		if (_server_config.getAutoindex() == 1)
			generateAutoindex();
		else if (_server_config.haveIndex())  {
			absPath = getFullPath();
			absPath += _server_config.getIndex();
		}// возможна ошибка

		else{
			setErrorCode(403);
			return false;
		}
			// error page
//	}
	
	if (checkCGI()) {
//		CGI _cgiResponse(_request, _server_config.getCGIpachByType(), );
//		_CGIResponse =  generateCGI();
//		setBody(_CGIResponse);
//		setContentLength(_CGIResponse.length());
//		setErrorCode(200);
//		setHeaders("Last-Modified", _lastModif);
//		setHeaders("Mime-Type", getMimeType(fullPath));
	}
	else {
		if (!fileExist(absPath)) {
			setErrorCode(404);
			return false;
		}
		std::stringstream  buff;
		std::ifstream file(absPath);
		if (!file.is_open())
			throw Response::FileCantOpenException();
		buff << file.rdbuf();
		file.close();
		std::string body = buff.str();
		setBody(body);
		setLastModif(absPath);
		setContentLength(body.length());
		setErrorCode(200);
		setHeaders("Last-Modified", _lastModif);
		setHeaders("Mime-Type", getMimeType(absPath));
	}

	return true;
}

bool Response::generatePUT() {
	std::ofstream out;
	struct stat st;
	if (stat(_fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode))
		out.open(_fullPath, std::ios_base::app);
	else
		out.open(_fullPath);
//	out << _request.get;
	setErrorCode(201);
	return true;
}

bool Response::generatePOST() {
	if (!checkCGI())
		return generatePUT();
//	CGI
	return true;
}

void Response::setErrorCode(int code) {
	_code = code;
}



void Response::setBody(std::string &body) {
	_body = body;
}


void Response::setDefaultHeader() {
//	setHeaders("");
	setDate();
	setHeaders("Content-Type", "text/html");
	setHeaders("Date", getDate());
	if (getHeader("Content-Length").empty())
		setHeaders("Content-Length", numberToString(_body.length()));
	setHeaders("Connection", "close");
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

std::string Response::generateCGI() {
	std::string str = "";
	setCGIResponse(str);
	return str;
}

bool Response::checkCGI() {
		std::string tmp = _request.getPath();
		size_t dot = tmp.find_last_of('.');
		std::string ext = tmp.substr(dot + 1);
		return (ext == "py" || ext == "js"); // или js
}

void Response::generateAutoindex() {

}

void Response::setCGIResponse(const std::string &str) {
	_CGIResponse = str;
}

int Response::getErrorCode() {
	return _code;
}

std::string Response::getMimeType(const std::string &file) {
	std::string end = file.substr(file.find_last_of('.') + 1);

	if (end == "html")
		return "text/html";
	if (end == "jpg")
		return "image/jpeg";
	if (end == "png")
		return "image/png";
	if (end == "css")
		return "text/css";
	if (end == "js")
		return "application/javascript";
	if (end == "py")
		return "application/octet-stream";
	return "text/plain";
}

void Response::errorPageFromFile(const std::string &path) {

	std::string f;
	std::stringstream  buff;
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Open file error\n";
	}
	buff << file.rdbuf();
	file.close();
	std::string body = buff.str();
	setBody(body);

	setHeaders("Content-Type", "text/html");
}

void Response::errorPageGenerator(int code) {

	std::string result;
	result += "<!DOCTYPE html>\n";
	result += "<html lang=\"en\">\n";
	result += "<head>\n";
	result += "<meta charset=\"UTF-8\">\n";
	result += "<title>Error " + numberToString(code) + "</title>\n";
	result += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x\" crossorigin=\"anonymous\">\n";
	result += "</head>\n";
	result += "<body>\n";
	result += "<div class=\"container\">\n";
	result += "<div class=\"alert alert-danger d-flex align-items-center mt-5\" role=\"alert\">\n";
	result += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"24\" height=\"24\" fill=\"currentColor\" class=\"bi bi-exclamation-triangle-fill flex-shrink-0 me-2\" viewBox=\"0 0 16 16\" role=\"img\" aria-label=\"Warning:\">\n";
	result += "<path d=\"M8.982 1.566a1.13 1.13 0 0 0-1.96 0L.165 13.233c-.457.778.091 1.767.98 1.767h13.713c.889 0 1.438-.99.98-1.767L8.982 1.566zM8 5c.535 0 .954.462.9.995l-.35 3.507a.552.552 0 0 1-1.1 0L7.1 5.995A.905.905 0 0 1 8 5zm.002 6a1 1 0 1 1 0 2 1 1 0 0 1 0-2z\"/>\n";
	result += "</svg>\n";
	result += "<div>\n<h3 class=\"mb-0\">" + numberToString(code) + "</h3>\n</div>\n";
	result += "</div>\n";
	result += "<div class=\"dropdown-divider mt-3\"></div>\n";
	result += "<div class=\"row mt-3\">\n";
	result += "<div class=\"col\"></div>\n";
	result += "<div class=\"col-auto\">\n<h5>Error " + _errors.at(code) +
			  " / Webserv</h5>\n</div>\n";
	result += "<div class=\"col\"></div>\n";
	result += "</div>\n</div>\n</body>\n</html>\n";
	setBody(result);
}

void Response::setFullPath() {
	std::string fullPath = _server_config.getRootByLocation(_request.getPath());
//	std::string fullPath(_server_config.getRoot() + _request.getPath());
	if (isDirectory(fullPath)) {
		if (fullPath.back() != '/')
			fullPath += "/";
		_fullPath = fullPath;
	}
}

std::string  Response::getFullPath() {
	return _fullPath;
}

const char *Response::FileCantOpenException::what() const throw() {
	return ("EXCEPTION! File can't open...");
};