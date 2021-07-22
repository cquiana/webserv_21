
#include "CGI.hpp"

CGI::CGI(std::map<std::string, std::string> headers,
		 const std::string &cgi_path,
		 const std::string &cgi_ext)
		 : _env(NULL), _headers(headers), _cgi_path(cgi_path), _cgi_ext(cgi_ext) {}

CGI::~CGI() {
	for (int i = 0; this->_env[i]; ++i) {
		delete[] this->_env[i];
	}
	delete[] this->_env;
}

const std::string &CGI::startCGI() {
	const std::string res;

	for(std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
		CGI::pushEnv(it->first, it->second);
	}

	pid_t pid;
	int In;
	int Out;
	int saveFdIn = dup(0);
	int saveFdOut = dup(1);

	pid = fork();
	if (pid == -1) {
		std::cerr << "Fork crashed." << std::endl;
	} else if (!pid) {
		char * const *nul = NULL;
		dup2(In, 0);
		dup2(Out, 1);
		execve(this->_cgi_path.c_str(), nul, this->_env);
	} else {
		char	buffer[100000] = {0};

		waitpid(-1, NULL, 0);

		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 100000);
			ret = read(Out, buffer, 100000 - 1);
			res += buffer;
		}
	}
	dup2(saveFdIn, 0);
	dup2(saveFdOut, 1);
	close(In);
	close(Out);
	close(saveFdIn);
	close(saveFdOut);
	return res;
}

void CGI::pushEnv(const std::string &key, const std::string &value) {
	unsigned int length = 0;
	char **res;
	char *text = (char*)calloc(strlen(key.c_str()) + strlen(value.c_str()) + 2, sizeof(char));

	strcpy(text, (key + "=" + value).c_str());
	if (this->_env) {
		for (int i = 0; this->_env[i]; i++) {
			length++;
		}
		length += 2;
		res = new char*[length];
		for (int i = 0; this->_env[i]; i++) {
			res[i] = strdup(this->_env[i]);
			delete[] this->_env[i];
		}
		delete[] this->_env;
		res[length - 2] = strdup(text);
		res[length - 1] = NULL;
		this->_env = res;
	} else {
		this->_env = new char*[2];
		this->_env[0] = strdup(text);
		this->_env[1] = NULL;
	}
}



//void CGI::POST() {
//	if (POST == cur_request._methodEnum)
//	{
//		if (_location.max_body > 0 && cur_request._body.length() > _location.max_body)
//			return (ReturnErrorPage(PayloadTooLadge, nullptr));
//		if (cur_request._body.length() == _location.max_body || (cur_request._body.length() == 0 && cur_request._queryString.empty()))
//		{
//			response_res.body = cur_request._body;
//			response_res.status_code_int_val = 200;
//			return response_res;
//		}
//
//		std::string execFile = std::string(_location.root + "/" + _location.exec);
//		int fdCgi = open (execFile.c_str(), O_RDONLY);
//		if (_location.exec.empty() || fdCgi == -1)
//		{
//			close(fdCgi);
//			return (ReturnErrorPage(PayloadTooLadge, nullptr));
//		}
//		close(fdCgi);
//		CGI cgi(execFile, _location, cur_request, *server_conf);
//		if (false == cgi.execve())
//			return (ReturnErrorPage(NotFound, nullptr));
//		cgi.parse();
//		response_res.status_code_int_val = cgi.getResponseIntStatus();
//		response_res.return_from_CGI = true;
//		response_res.body = cgi.getResponseBody();
//		response_res.headers["CONTENT_TYPE"] = cgi.getResponseContentType();
//
//		return response_res;
//	}
//};











//CGI::CGI(std::string const &filename, Location const & location, Request const & request, Serverconf const &config) :
//_filename(filename), _location(location), _request(request), _config(config), _envp(NULL)
//{
//	setupEnv();
//	_response_body = "";
//	_response_content_type = "";
//	_response_int_status = 0;
//}

bool CGI::execve() {
	int status, ret;
	int fd1;
	int fd0;

	const char* cgipath = _filename.c_str();
	const char* argv[] = { const_cast<char *>(cgipath), NULL};
	char buff[BUFFER_SIZE_CGI];

	std::ofstream file_body;
	file_body.open("bodytmp");
	file_body << _request.body;
	file_body.close();

	if ((fd0 = open("bodytmp", O_RDONLY)) < 0)
		return false;

	if ((fd1 = open("filetmp", O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD, 0755)) < 0)
		return false;

	pid_t pid = fork();
	if (!pid)
	{
		dup2(fd0, 0);
		dup2(fd1, 1);
		if (execve(cgipath, argv, _envp) == -1)
		{
			close(fd1);
			close(fd0);
			remove("bodytmp");
			deleteEnv(_envp);
			exit(1);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		close(fd1);
		close(fd0);
		fd1 = open("filetmp", O_RDONLY);

		for (ret = BUFFER_SIZE_CGI; ret == BUFFER_SIZE_CGI; _response_body += std::string(buff, ret))  //ToDo
		{
			if ((ret = read(fd1, buff, BUFFER_SIZE_CGI) == -1)
			{
				close(fd);
				remove("filetmp");
				remove("bodytmp");
				deleteEnv(_envp);
				return false;
			}
			close(fd);
		}
	}

	//ToDo
	remove("filetmp");
	remove("bodytmp");
	deleteEnv(_envp);
	return true;
}

void CGI::parse() {
	size_t header_end = _response_body.find("\r\n\r\n");
	std::string header = _response_body.substr(0, header_end);
	size_t i = header.find("Status: ") + 8;
	if (isdigit(header[i]))
	{
		while (header[i] >= '0' && header[i] <= '9')
		{
			_response_int_status = _response_int_status * 10 + (header[i] - '0');
			i++;
		}
		i++;
		size_t j = header.find("\r\n") - i;
		_response_content_type = header.substr(i + j + 2, header.size() - 2);
		_response_body.erase(0, header_end + 4);
	}
	size_t body = _response_body.size();
	(void)body;
}

void CGI::setupEnv() {
	std::vector<std::string> envVectorString;

	envVectorString.push_back("AUTH_TYPE=" + _location.auth);
	envVectorString.push_back("CONTENT_LENGHT=" + std::to_string(_request._body.length()));
	envVectorString.push_back("CONTENT_TYPE=" + _request.getValueOfHeaders("CONTENT_TYPE"));
	envVectorString.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVectorString.push_back("PATH_INFO=" + _location.location);
	envVectorString.push_back("PATH_TRANSLATED=" + _location.root + _location.location);
	envVectorString.push_back("QUERY_STRING=" + _request._query_string);
	envVectorString.push_back("REMOTE_ADDR=" + _config.ip);
	envVectorString.push_back("REMOTE_USER=" + _request._user);
	envVectorString.push_back("REMOTE_IDENT=" + _request._user + "." + _request.getValueOfHeaders("host"));
	envVectorString.push_back("REQUEST_METHOD=" + _request._methodStr);
	envVectorString.push_back("REQUEST_URI=" + _location.location);
	envVectorString.push_back("SCRIPT_NAME=" + _location.exec);
	envVectorString.push_back("SERVER_NAME=" + _config.server_name);
	envVectorString.push_back("SERVER_PORT=" + std::to_string(_config.port));
	envVectorString.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envVectorString.push_back("SERVER_SOFTWARE=Webserv21");

	for (std::map<std::string,std::string>::const_iterator it = _request._header.begin(); it != _request._header.end(); it++)
		envVectorString.push_back("HTTP_" + std::string(it->first + "=" + it->second);

	_envp = convertEnvp(envVectorString);
}

char** CGI::convertEnvp(std::vector<std::string> envVectorString) {
	char** ret = new char* [envVectorString.size() + 1];
	for (size_t i = 0; i < envVectorString.size() - 1; i++)
		ret[i] = strdup(envVectorString[i].c_str());
	ret[envVectorString.size()] = NULL;
	return ret;
}

void CGI::deleteEnv(char** envp) {
	size_t i = 0;
	while (envp[i])
		delete envp[i++];
	delete[] envp;
}

int CGI::getResponseIntStatus() {
	return _response_int_status;
}

int CGI::getResponseBody() {
	return _response_body;
}

int CGI::getResponseContentType() {
	return _response_content_type;
}

