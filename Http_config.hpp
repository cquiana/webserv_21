//
// Created by demon on 22.06.2021.
//

#ifndef WEBSERVER_HTTP_CONFIG_HPP
# define WEBSERVER_HTTP_CONFIG_HPP

#include <string>
#include <vector>
#include "Server_config.hpp"

class Http_config
{
private:
	int _client_max_body_size;
	std::vector<int> _error_page_ints;
	std::vector<std::string> _error_page_strings;
	std::vector<Server_config> _servers;
	std::vector<std::string> _servers_names;

	Http_config(Http_config const &another);
	Http_config &operator=(Http_config const &another);

public:
	Http_config();
	~Http_config();

	bool haveErrorPage(int page) const;
	bool haveMaxBody() const;
	bool haveServer(std::string servers_name) const;

	bool haveSomeServer() const;

	std::string getErrorPage(int page) const;
	int getMaxBody() const;
	Server_config* getServer(std::string servers_name) const;

	void setErrorPage(int error_page_int, std::string error_page_string);
	void setMaxBody(int max_body);
	//int setServer(std::string servers_name, Server_config serverConfig);

	class PageAlreadySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class MaxBodyAlreadySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerExistException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class ErrorPageNotExistException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class ErrorPageAlreadyExistException: public std::exception {
		virtual const char *what() const throw() ;
	};
};

#endif //WEBSERVER_HTTP_CONFIG_HPP
