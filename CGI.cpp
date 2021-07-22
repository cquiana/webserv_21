#include "CGI.hpp"

CGI::CGI(Request &request, Server_config &server_config) : _request(request), _server_config(server_config), _env(NULL) , _out_status_int_code(0), _out_string_content_type(""), _out_string_body_to_response("") {
	setupEnv();
}

CGI::~CGI() {}

bool CGI::execveCGI() {
	int fd1;
	int fd0;

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


//	std::cout << "##### " << _request.getQueryString() << " @@@@@\n";

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
//		if (execve(_request.getPath().c_str(), nullptr, _env) == -1)
//		std::string tmp = _request.getPath();
//		size_t dot = tmp.find_last_of('.');
//		std::string ext = tmp.substr(dot + 1);
//
//		std::cout << "##### " << _request.getPath() << " @@@@@\n";
//		exit(1);

//		std::string a = _server_config.getCGIpachByType(ext) + " " + fullPath;
//		std::cerr << a << std::endl;

//		execve(a.c_str(), argv, _env);
		execve(fullPath.c_str(), argv, _env);
//		if (execve(fullPath.c_str(), NULL, NULL) == -1)
//			std::cerr << "ERROR !!! Fork crashed3..." << std::endl;
//		std::cerr << errno << std::endl;
//		std::cerr << "ERROR !!! Fork crashed2..." << std::endl;
		std::cerr << "execve ERROR" << std::endl;
		_out_status_int_code = 500;
		close(fd1);
		close(fd0);
		remove("In_Body.tmp");
		deleteEnv(_env);
		exit(1);
	}
	else if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
		lseek(fd1, 0, SEEK_SET);

//		std::cout << "##### " << pid << " @@@@@\n";

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
			_out_string_body_to_response += buff;
		}
	}

//	std::cout << "##### " << _out_string_body_to_response << " end @@@@@\n";
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

