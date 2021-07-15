//
// Created by demon on 23.06.2021.
//

#include "Location_config.hpp"
#include <iostream>

Location_config::~Location_config() {}

Location_config::Location_config(std::string location_prefix, std::string type) :
		_location_prefix(location_prefix), _type(type), _cgi_path(""), _root(""), _methods(0), _IsCGI(false) , _IsPrefix(false) {
	//std::cout << "Type ##_" << _type << "##_location_prefix ##_" << _location_prefix << "_## \n";
	if (location_prefix == "" && type != "")
		_IsCGI = true;
	else if (location_prefix != "" && type == "")
		_IsPrefix = true;
	else
		throw Location_config::SetLocationException();
}

Location_config::Location_config(Location_config const &another) :
		_location_prefix(another._location_prefix), _type(another._type), _cgi_path(another._cgi_path), _root(another._root), _methods(another._methods), _IsCGI(another._IsCGI) , _IsPrefix(another._IsPrefix) {}

Location_config& Location_config::operator=(Location_config const &another) {
	_location_prefix = another._location_prefix;
	_type = another._type;
	_cgi_path = another._cgi_path;
	_root = another._root;
	_methods =another._methods;
//	_some = another._some;
	_IsCGI = another._IsCGI;
	_IsPrefix = another._IsPrefix;
	return *this;
}

//bool Location_config::isType(std::string type) const {
//	if (std::find(_types.begin(), _types.end(), type) != _types.end())
//		return true;
//	else
//		return false;
//}

bool Location_config::mIsCGI() const {
	if (_IsCGI)
		return true;
	else
		return false;
}

bool Location_config::mIsPrefic() const {
	if (_IsPrefix)
		return true;
	else
		return false;
}

bool Location_config::haveRoot() const {
	return (!(_root.empty()));
}

bool Location_config::haveCgiPath() const {
	return (!(_cgi_path.empty()));
}

bool Location_config::haveMethods() const {
	if (_methods != 0)
		return true;
	else
		return false;
}

bool Location_config::haveType(std::string type) const {
//	if (std::find(_types.begin(), _types.end(), type) != _types.end())
	if (type == _type)
		return true;
	else
		return false;
}

bool Location_config::prefixCheck(std::string prefix) const {
	size_t found_pattern = prefix.find(_location_prefix);
	if (found_pattern != std::string::npos && found_pattern < 4)
		return true;
	else
		return false;
}


std::string Location_config::getType() const {
	//std::cout << "Type ##" << _type << "##\n";
	return _type;
}

std::string Location_config::getLocationPrefix() const {
	return _location_prefix;
}

std::string Location_config::getRoot() const{
	return _root;
}

std::string Location_config::getCgiPath() const{
	return _cgi_path;
}

int Location_config::getMethods() const{
	return _methods;
}

void Location_config::setRoot(std::string root){
	if (haveRoot())
		throw Location_config::RootAlraedySetException();
	_root = root;
//	_some = true;
}

void Location_config::setCgiPath(std::string cgi_path) {
	if (haveCgiPath())
		throw Location_config::CgiPathAlraedySetException();
	_cgi_path = cgi_path;
//	_some = true;
}

void Location_config::setMethods(int methods) {
	if (haveMethods())
		throw Location_config::MethodsAlraedySetException();
	_methods = methods;
//	_some = true;
}

const char *Location_config::RootAlraedySetException::what() const throw() {
	return ("EXCEPTION! Root Alraedy Set in this Location...");
};

const char *Location_config::CgiPathAlraedySetException::what() const throw() {
	return ("EXCEPTION! FastCgi Alraedy Set in this Location...");
};

const char *Location_config::MethodsAlraedySetException::what() const throw() {
	return ("EXCEPTION! Methods Alraedy Set in this Location...");
};

const char *Location_config::SetLocationException::what() const throw() {
	return ("EXCEPTION! Type and prefix missmath...");
};