//
// Created by demon on 22.06.2021.
//

#ifndef WEBSERVER_SERVER_CONFIG_HPP
# define WEBSERVER_SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include <iostream>
#include "Location_config.hpp"

class Server_config
{
private:
	int _port;
	std::string _name;
	std::string _root;
	std::string _index;
	int _autoindex;
	int _return_code;
	std::string _return_adress;
	int _active_location;

public:
	Server_config();
	~Server_config();
	Server_config(Server_config const &another);
	Server_config &operator=(Server_config const &another);

	std::vector<Location_config> _locations;

	bool havePort() const;
	bool haveName() const;
	bool haveRoot() const;
	bool haveIndex() const;
	bool haveAutoindex() const;
	bool haveLocation() const;
	bool haveReturnCode() const;
    bool blockedReturnCode() const;
    bool haveActiveLocation() const;

	int getPort() const;
	std::string getName() const;
	std::string getRoot() const;
	std::string getIndex() const;
	int getAutoindex() const;
	int getReturnCode() const;
	std::string getReturnArdess() const;
	int getActiveLocation() const;

//	std::string getRootByLocation(std::string type, std::string loc);
	std::string getRootByLocation(std::string loc);

	void setPort(int port);
	void setName(std::string name);
	void setRoot(std::string root);
	void setIndex(std::string index);
	void setAutoindex(int autoindex);
	void setReturnCode(int return_code, std::string return_adress);

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
	class LocationNotOpenedException:		public std::exception {
		virtual const char *what() const throw() ;
	};


//	class CodePageNumberAlraedySetException: public std::exception {
//		virtual const char *what() const throw() ;
//	};
};

#endif //WEBSERVER_SERVER_CONFIG_HPP
