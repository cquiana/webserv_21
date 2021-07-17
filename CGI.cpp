#include "CGI.hpp"

CGI::CGI(std::map<std::string, std::string> headers) : _env(NULL), _headers(headers) {}

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

void CGI::Start() {
	CGI::pushEnv("Content-Type", "text");
	CGI::pushEnv("QUERY_STRING", this->_request);

	for (int i = 0; this->_env[i]; ++i) {
		std::cout << this->_env[i] << std::endl;
	}
}

int main() {
	CGI local("alex=23");

	local.Start();
}
