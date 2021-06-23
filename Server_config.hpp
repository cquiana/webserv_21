//
// Created by demon on 22.06.2021.
//

#ifndef WEBSERVER_SERVER_CONFIG_HPP
# define WEBSERVER_SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include "Location_config.hpp"

class Server_config
{
private:
	int _port;
	std::string _name;
	std::string _root;
	std::string _index;
	int _autoindex;
	std::vector<Location_config*> _locations;
	int _return_code;
	std::string _return_adress;

	Server_config(Server_config const &another);
	Server_config &operator=(Server_config const &another);

public:
	Server_config();
	~Server_config();

	bool havePort() const;
	bool haveName() const;
	bool haveRoot() const;
	bool haveIndex() const;
	bool haveAutoindex() const;
	bool haveLocation() const;
	bool haveReturnCode() const;

	int getPort() const;
	std::string getName() const;
	std::string getRoot() const;
	std::string getIndex() const;
	int getAutoindex() const;
	int getReturnCode() const;
	std::string getReturnArdess() const;

	std::string getRootByLocation(std::string type) const;

	void setPort(int port);
	void setName(std::string name);
	void setRoot(std::string root);
	void setIndex(std::string index);
	void setAutoindex(int autoindex);
	void setReturnCode(int return_code, std::string return_adress);

	void setLocation();

	class PortAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class NameAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class RootAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class IndexAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class AutoindexAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class ReturnCodeAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
//	class CodePageNumberAlraedySetException: public std::exception {
//		virtual const char *what() const throw() ;
//	};
};

#endif //WEBSERVER_SERVER_CONFIG_HPP
