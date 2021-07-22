
#include "CGI.hpp"

//CGI::CGI(std::map<std::string, std::string> headers,
//		 const std::string &cgi_path,
//		 const std::string &cgi_ext)
//		 : _env(NULL), _headers(headers), _cgi_path(cgi_path), _cgi_ext(cgi_ext) {}

CGI::CGI(Request &request, Server_config &server_config) : _request(request), _server_config(server_config), _env(NULL) , _out_status_int_code(0), _out_string_content_type(""), _out_string_body_to_response("") {
	setupEnv();
}

CGI::~CGI() {
	//deleteEnv(_env);
}

//const std::string &CGI::startCGI() {
//	const std::string res;
//
//	for(std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
//		CGI::pushEnv(it->first, it->second);
//	}
//
//	pid_t pid;
//	int In;
//	int Out;
//	int saveFdIn = dup(0);
//	int saveFdOut = dup(1);
//
//	pid = fork();
//	if (pid == -1) {
//		std::cerr << "Fork crashed." << std::endl;
//		//ToDo 500error here
//	} else if (!pid) {
//		char * const *nul = NULL;
//		dup2(In, 0);
//		dup2(Out, 1);
//		execve(this->_cgi_path.c_str(), nul, this->_env);
//	} else {
//		char	buffer[100000] = {0};
//
//		waitpid(-1, NULL, 0);
//
//		int ret = 1;
//		while (ret > 0)
//		{
//			memset(buffer, 0, 100000);
//			ret = read(Out, buffer, 100000 - 1);
//			res += buffer;
//		}
//	}
//	dup2(saveFdIn, 0);
//	dup2(saveFdOut, 1);
//	close(In);
//	close(Out);
//	close(saveFdIn);
//	close(saveFdOut);
//	return res;
//}

//void CGI::pushEnv(const std::string &key, const std::string &value) {
//	unsigned int length = 0;
//	char **res;
//	char *text = (char*)calloc(strlen(key.c_str()) + strlen(value.c_str()) + 2, sizeof(char));
//
//	strcpy(text, (key + "=" + value).c_str());
//	if (this->_env) {
//		for (int i = 0; this->_env[i]; i++) {
//			length++;
//		}
//		length += 2;
//		res = new char*[length];
//		for (int i = 0; this->_env[i]; i++) {
//			res[i] = strdup(this->_env[i]);
//			delete[] this->_env[i];
//		}
//		delete[] this->_env;
//		res[length - 2] = strdup(text);
//		res[length - 1] = NULL;
//		this->_env = res;
//	} else {
//		this->_env = new char*[2];
//		this->_env[0] = strdup(text);
//		this->_env[1] = NULL;
//	}
//}



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

bool CGI::execveCGI() {
	int fd1;
	int fd0;

//	const char* cgipath = _request.getPath().c_str();
//	const char* argv[] = { const_cast<char *>(cgipath), NULL};
	char buff[_server_config.getMaxBody()];

	std::ofstream file_body;
	file_body.open("In_Body.tmp");
	file_body << _request.getReqBody();
	file_body.close();

	if ((fd0 = open("In_Body.tmp", O_RDONLY)) < 0)
		return false;

	if ((fd1 = open("Out_file.tmp", O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD, 0755)) < 0)
		return false;

	std::string fullPath(_server_config.getRoot() + _request.getPath());
	if (isDirectory(fullPath)) {
		if (fullPath[fullPath.length() - 1] != '/')
			fullPath += "/";
	}
	char *const argv [] = {const_cast<char *>(fullPath.c_str()), NULL};


//	std::string tmp = _request.getPath();
//	size_t dot = tmp.find_last_of('.');
//	std::string ext = tmp.substr(dot + 1);
//	std::cout << "##### " << _request.getPath() << " @@@@@\n";
//	std::cout << "##### " << _server_config.getCGIpachByType(ext).c_str() << " @@@@@\n";
//	std::cout << "##### " << fullPath << " @@@@@\n";

	pid_t pid = fork();
	if (pid < 0)
	{
		std::cerr << "ERROR !!! Fork crashed1..." << std::endl;
		_out_status_int_code = 500;
		return false;
	}
	else if (pid == 0)
	{
		dup2(fd0, 0);
		dup2(fd1, 1);
//		if (execve(cgipath, argv, _env) == -1)
//		if (execve(_request.getPath().c_str(), nullptr, _env) == -1)
//		std::string tmp = _request.getPath();
//		size_t dot = tmp.find_last_of('.');
//		std::string ext = tmp.substr(dot + 1);
//
//		std::cout << "##### " << _request.getPath() << " @@@@@\n";
//		exit(1);

//		std::string a = _server_config.getCGIpachByType(ext) + " " + fullPath;
//		std::cerr << a << std::endl;

		execve(fullPath.c_str(), argv, _env);
//		if (execve(fullPath.c_str(), NULL, NULL) == -1)
//			std::cerr << "ERROR !!! Fork crashed3..." << std::endl;
//		std::cerr << errno << std::endl;
//		std::cerr << "ERROR !!! Fork crashed2..." << std::endl;
		_out_status_int_code = 500;
		close(fd1);
		close(fd0);
		remove("In_Body.tmp");
		deleteEnv(_env);
		exit(1);
//		if (execve(_server_config.getCGIpachByType(ext).c_str(), nullptr, _env) == -1)
//		{
//			close(fd1);
//			close(fd0);
//			remove("In_Body.tmp");
//			deleteEnv(_env);
//			exit(1);
//		}
	}
	else if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
		lseek(fd1, 0, SEEK_SET);

		std::cout << "##### " << pid << " @@@@@\n";

		int ret = 1;
		while (ret > 0)
		{
			memset(buff, 0, _server_config.getMaxBody());
			ret = read(fd1, buff, _server_config.getMaxBody() - 1);
			if (ret == -1)
			{
				close(fd1);
				remove("Out_file.tmp");
				remove("In_Body.tmp");
				deleteEnv(_env);
				return false;
			}
//			newBody += buff;
			_out_string_body_to_response += buff;
		}


//		waitpid(pid, &status, 0);
//		close(fd1);
//		close(fd0);
//		fd1 = open("filetmp", O_RDONLY);
//
//		for (ret = BUFFER_SIZE_CGI; ret == BUFFER_SIZE_CGI; _response_body += std::string(buff, ret))  //ToDo
//		{
//			if ((ret = read(fd1, buff, BUFFER_SIZE_CGI) == -1)
//			{
//				close(fd1);
//				remove("filetmp");
//				remove("bodytmp");
//				deleteEnv(_envp);
//				return false;
//			}
//			close(fd1);
//		}
	}

	//ToDo
	remove("Out_file.tmp");
	remove("In_Body.tmp");
	if (_env)
		deleteEnv(_env);
	return true;
}

void CGI::parse() {
	size_t header_end = _out_string_body_to_response.find("\r\n\r\n");
	std::string header = _out_string_body_to_response.substr(0, header_end);
	size_t find_status = header.find("Status: ") + 8;
	if (isdigit(header[find_status]))
	{
		while (header[find_status] >= '0' && header[find_status] <= '9')
		{
			_out_status_int_code = _out_status_int_code * 10 + (header[find_status] - '0');
			find_status++;
		}
		find_status++;
		size_t a = header.find("\r\n") - find_status;
		_out_string_content_type = header.substr(find_status + a + 2, header.size() - 2);
		_out_string_body_to_response.erase(0, header_end + 4);
	}
	size_t body = _out_string_body_to_response.size();
	(void)body;
}

void CGI::setupEnv() {
	std::vector<std::string> envVectorString;

	envVectorString.push_back("AUTH_TYPE=" + _request.getAuthType());
	envVectorString.push_back("CONTENT_LENGHT=" + std::to_string(_request.getContentLength()));
	envVectorString.push_back("CONTENT_TYPE=" + _request._headers["content_type"]);
	envVectorString.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVectorString.push_back("PATH_INFO=" + _request.getPath());
	envVectorString.push_back("PATH_TRANSLATED=" + _server_config.getRootByLocation(_request.getPath()) + _request.getPath());
	envVectorString.push_back("QUERY_STRING=" + _request.getQueryString());
	envVectorString.push_back((std::string("REMOTE_ADDR=") + IP).c_str());
//	envVectorString.push_back("REMOTE_USER=" + _request._user);
//	envVectorString.push_back("REMOTE_IDENT=" + _request._user + "." + _request.getValueOfHeaders("host"));
	envVectorString.push_back("REQUEST_METHOD=" + _request.getMethod());
	envVectorString.push_back("REQUEST_URI=" + _request.getPath());
	envVectorString.push_back("SCRIPT_NAME=" + _request.getPath());
	envVectorString.push_back("SERVER_NAME=" + _server_config.getName());
	envVectorString.push_back("SERVER_PORT=" + numberToString(_server_config.getPort()));
	envVectorString.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envVectorString.push_back("SERVER_SOFTWARE=Webserv21");

	for (std::map<std::string,std::string>::const_iterator it = _request._headers.begin(); it != _request._headers.end(); it++)
		envVectorString.push_back("HTTP_" + std::string(it->first + "=" + it->second));

	_env = new char* [envVectorString.size() + 1];
	for (size_t i = 0; i < envVectorString.size(); i++)
		_env[i] = strdup(envVectorString[i].c_str());
	_env[envVectorString.size()] = NULL;
//	std::cout << envVectorString.size() << " # " << envVectorString.size() + 1 << "\n\n";
//	size_t j = 0;
//	while (_env[j])
//	{
//		std::cout << _env[j] << "\n";
//		j++;
//	}
//	std::cout << "\n\n";
//	_envp = convertEnvp(envVectorString);
}

//char** CGI::convertEnvp(std::vector<std::string> envVectorString) {
//	char** ret = new char* [envVectorString.size() + 1];
//	for (size_t i = 0; i < envVectorString.size() - 1; i++)
//		ret[i] = strdup(envVectorString[i].c_str());
//	ret[envVectorString.size()] = NULL;
//	return ret;
//}

void CGI::deleteEnv(char** envp) {

//	std::cout << envVectorString.size() << " # " << envVectorString.size() + 1 << "\n\n";
//	std::cout << "\n\n";
//	size_t j = 0;
//	while (_env[j])
//	{
//		std::cout << _env[j] << "\n";
//		j++;
//	}
//	std::cout << "\n\n";
	if (envp)
	{
		size_t i = 0;
		while (envp[i])
		{
//			std::cout << envp[i] << "\n";
			delete[] envp[i];
			i++;
		}
		delete[] envp;
	}
}

int CGI::getOutStatusIntCode() {
	return _out_status_int_code;
}

std::string& CGI::getOutStringBodyToResponse() {
	return _out_string_body_to_response;
}

std::string CGI::getOutStringContentType() {
	return _out_string_content_type;
}

