//
// Created by demon on 23.06.2021.
//

#include "Location_config.hpp"

Location_config::~Location_config() {}

Location_config::Location_config(std::string location_path, std::vector<std::string> types) :
		_location_path(location_path), _types(types), _cgi_path(""), _root(""), _methods(0), _some(false) {}

Location_config::Location_config(Location_config const &another) :
	_location_path(another._location_path), _types(another._types), _cgi_path(another._cgi_path), _root(another._root), _methods(another._methods), _some(another._some) {}

Location_config& Location_config::operator=(Location_config const &another) {
	_location_path = another._location_path;
	_types = another._types;
	_cgi_path = another._cgi_path;
	_root = another._root;
	_methods =another._methods;
	_some = another._some;
	return *this;
}

bool Location_config::isType(std::string type) const {
	if (std::find(_types.begin(), _types.end(), type) != _types.end())
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

bool Location_config::methodsNull() const {
	if (_methods != 0)
		return false;
	else
		return true;
}

bool Location_config::haveType(std::string type) const {
	if (std::find(_types.begin(), _types.end(), type) != _types.end())
		return true;
	else
		return false;
}

std::vector<std::string> Location_config::getTypes() const {
	return _types;
}

std::string Location_config::getLocationPath() const{
	return _location_path;
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
	_some = true;
}

void Location_config::setCgiPath(std::string cgi_path) {
	if (haveCgiPath())
		throw Location_config::CgiPathAlraedySetException();
	_cgi_path = cgi_path;
	_some = true;
}

void Location_config::setMethods(int methods) {
	if (methodsNull())
		throw Location_config::MethodsAlraedySetException();
	_methods = methods;
	_some = true;
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