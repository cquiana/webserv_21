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
	bool _active_server;
	std::string _wd;

public:
	Http_config();
	Http_config(Http_config const &another);
	Http_config &operator=(Http_config const &another);
	~Http_config();

	std::vector<Server_config> _servers;

	bool haveMaxBody() const;
	bool haveServer(std::string servers_name);

	bool haveSomeServer() const;
	bool haveActiveServer() const;

	int getMaxBody() const;
	Server_config getServer(std::string servers_name);
	int getActiveServer() const;
	std::string getWD();

	void setMaxBody(int max_body);
	void setWD(std::string wd);

	void setMaxBodyToServers();

	void addServer();
	void checkLastServeer();

	class PageAlreadySetException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class MaxBodyAlreadySetException:		public std::exception {
		virtual const char *what() const throw() ;
	};
	class MaxBodyWrongSetException:		public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerExistException:				public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerNotFoundException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerNotOpenedException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class SizeServersException:				public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerAlreadyOpenedException:		public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerSocketException:		public std::exception {
		virtual const char *what() const throw() ;
	};


};

#endif //WEBSERVER_HTTP_CONFIG_HPP
