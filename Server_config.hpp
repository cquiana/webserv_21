//
// Created by demon on 22.06.2021.
//

#ifndef WEBSERVER_SERVER_CONFIG_HPP
# define WEBSERVER_SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include <iostream>
#include <netinet/in.h>
#include "Location_config.hpp"
#include <fcntl.h>

class Server_config
{
private:
	int _port;
	std::string _name;
	std::string _root;
	std::string _index;
	int _socket;
	int _autoindex;
	int _return_code; // -1 ne zadan, 0 zablokirovan, >0 = code
	std::string _return_adress;
	bool _active_location;

	struct sockaddr_in _addr;

public:
	Server_config();
	~Server_config();
	Server_config(Server_config const &another);
	Server_config &operator=(Server_config const &another);

	std::vector<Location_config> _locations;
	std::vector<int> _error_page_ints;
	std::vector<std::string> _error_page_strings;

	bool haveErrorPage(int page) const;
	bool havePort() const;
	bool haveName() const;
	bool haveRoot() const;
	bool haveIndex() const;
	bool haveSocket() const;
	bool haveAutoindex() const;
	bool haveLocation() const;
	bool haveReturnCode() const;
    bool blockedReturnCode() const;
    bool haveActiveLocation() const;

	std::vector<int> getAllErrorPagesInts() const;
	std::vector<std::string> getAllErrorPages() const;
	std::string getErrorPage(int page) ; //const not needed, else it error compile
	int getPort() const;
	std::string getName() const;
	std::string getRoot() const;
	std::string getIndex() const;
	int getAutoindex() const;
	int getSocket() const;
	int getReturnCode() const;
	std::string getReturnArdess() const;
	int getActiveLocation() const;

//	std::string getRootByLocation(std::string type, std::string loc);
	std::string getRootByLocation(std::string loc);
	bool checkCGIbyType(std::string cgi);
	std::string getCGIpachByType(std::string cgi);

	void setPort(int port);
	void setName(std::string name);
	void setRoot(std::string root);
	void setSocket(int sock);
	void setIndex(std::string index);
	void setAutoindex(int autoindex);
	void setReturnCode(int return_code, std::string return_adress);
	void setErrorPage(size_t error_page_int, std::string error_page_string);

	void initSocket();
	int acceptNewConnect();
	void addLocation(std::string location_path, std::string type);
	void checkLastLocation();

	class PortAlraedySetException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class NameAlraedySetException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class RootAlraedySetException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class IndexAlraedySetException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class AutoindexAlraedySetException:		public std::exception {
		virtual const char *what() const throw() ;
	};
	class ReturnCodeAlraedySetException:	public std::exception {
		virtual const char *what() const throw() ;
	};
    class BlockedReturnCodeException:		public std::exception {
        virtual const char *what() const throw() ;
    };
	class SizeLocationsException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class AnotherLocationOpenedException:	public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerSocketInitError:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerSocketException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class ServerCGInotFoundException:		public std::exception {
		virtual const char *what() const throw() ;
	};
	class ErrorPageNotExistException:		public std::exception {
		virtual const char *what() const throw() ;
	};
	class ErrorPageAlreadyExistException:	public std::exception {
		virtual const char *what() const throw() ;
	};



//	class CodePageNumberAlraedySetException: public std::exception {
//		virtual const char *what() const throw() ;
//	};
};

#endif //WEBSERVER_SERVER_CONFIG_HPP
