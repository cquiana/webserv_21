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
	std::string _location_prefix;
	std::string _type;
	std::string _cgi_path;
	std::string _root;
	int _methods;  //ToDo 1 + 2 + 4 tri methoda
//	bool _some;
	bool _IsCGI;
	bool _IsPrefix;

public:
	~Location_config();
	Location_config(std::string _location_prefix, std::string types);
	Location_config(Location_config const &another);
	Location_config &operator=(Location_config const &another);

//	bool isType(std::string type) const;
	bool mIsCGI() const;
	bool mIsPrefic() const;

	bool haveRoot() const;
	bool haveCgiPath() const;
	bool methodsNull() const;
	bool haveType(std::string type) const;
	bool prefixCheck(std::string prefix) const;

	std::string getType() const;
	std::string getLocationPrefix() const;
	std::string getRoot() const;
	std::string getCgiPath() const;
	int getMethods() const;

	void setRoot(std::string root);
	void setCgiPath(std::string cgi_path);
	void setMethods(int methods);

	class RootAlraedySetException:			public std::exception {
		virtual const char *what() const throw() ;
	};
	class CgiPathAlraedySetException:		public std::exception {
		virtual const char *what() const throw() ;
	};
	class MethodsAlraedySetException:		public std::exception {
		virtual const char *what() const throw() ;
	};
	class SetLocationException:				public std::exception {
		virtual const char *what() const throw() ;
	};

};

#endif //WEBSERVER_LOCATION_CONFIG_HPP
