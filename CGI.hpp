#ifndef CGI_HPP
#define CGI_HPP

#include "Webserv.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <map>
#include <unistd.h>
#include <sys/wait.h>

class CGI {
private:
	CGI();
	char**  _env;
	Request _request;
	Server_config _server_config;
	std::string _out_string_body_to_response;
	std::string _out_string_content_type;
	int _out_status_int_code;


//	std::map<std::string, std::string> _headers;
//	const std::string &_cgi_path;
//	const std::string &_cgi_ext;

//	void pushEnv(const std::string &key, const std::string &value);
public:
	CGI(Request &request, Server_config &server_config);
	~CGI();


	bool execveCGI();
	void parse();
	void setupEnv();
//	char** convertEnvp(std::vector<std::string> envVectorString);
	void deleteEnv(char** envp);


	int getOutStatusIntCode();
	std::string& getOutStringBodyToResponse();
	std::string getOutStringContentType();


	const std::string &startCGI();
};

#endif
