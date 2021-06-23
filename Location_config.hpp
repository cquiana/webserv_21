//
// Created by demon on 22.06.2021.
//

#ifndef WEBSERVER_LOCATION_CONFIG_HPP
# define WEBSERVER_LOCATION_CONFIG_HPP

#include <string>
#include <vector>
#include <algorithm>

class Location_config
{
private:
	Location_config();
	Location_config(Location_config const &another);
	Location_config &operator=(Location_config const &another);
	std::vector<std::string> _types;
	std::string _fastcgi_pass;
	std::string _root;
	int _methods;

public:
	~Location_config();
	Location_config(std::vector<std::string> types, std::string fastcgi_pass, std::string root, int methods);

	bool isType(std::string type) const;

	bool haveRoot() const;
	bool haveFastCgi() const;
	bool methodsNull() const;

	std::vector<std::string> getTypes() const;
	std::string getRoot() const;
	std::string getFastCgi() const;
	int getMethods() const;

	void setRoot(std::string root);
	void setFastCgi(std::string fastcgi_pass);
	void setMethods(int methods);

	class RootAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class FastCgiAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
	class MethodsAlraedySetException: public std::exception {
		virtual const char *what() const throw() ;
	};
};

#endif //WEBSERVER_LOCATION_CONFIG_HPP
