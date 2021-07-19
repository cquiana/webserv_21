#include "CGI.hpp"

CGI::CGI(std::map<std::string, std::string> headers,
		 const std::string &cgi_path,
		 const std::string &cgi_ext)
		 : _env(NULL), _headers(headers), _cgi_path(cgi_path), _cgi_ext(cgi_ext) {}

const std::string &CGI::startCGI() {
	const std::string &res;

	for(auto it = headers.begin(); it != headers.end(); ++it) {
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
		execve(this->cgi_path.c_str(), nul, this->_env);
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

CGI::~CGI() {
	for (int i = 0; this->_env[i]; ++i) {
		delete[] this->_env[i];
	}
	delete[] this->_env;
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
