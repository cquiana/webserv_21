//
// Created by demon on 23.06.2021.
//

#include "Location_config.hpp"

Location_config::~Location_config() {}

Location_config::Location_config(std::vector<std::string> types, std::string fastcgi_pass, std::string root, int methods) :
		_types(types), _fastcgi_pass(""), _root(""), _methods(0) {}

bool Location_config::isType(std::string type) const {
	if (std::find(_types.begin(), _types.end(), type) != _types.end())
		return true;
	else
		return false;
}

bool Location_config::haveRoot() const {
	return (!(_root.empty()));
}

bool Location_config::haveFastCgi() const {
	return (!(_fastcgi_pass.empty()));
}

bool Location_config::methodsNull() const
{
	if (_methods != 0)
		return false;
	else
		return true;
}

std::vector<std::string> Location_config::getTypes() const {
	return _types;
}

std::string Location_config::getRoot() const{
	return _root;
}

std::string Location_config::getFastCgi() const{
	return _fastcgi_pass;
}

int Location_config::getMethods() const{
	return _methods;
}

void Location_config::setRoot(std::string root){
	if (haveRoot())
		throw Location_config::RootAlraedySetException();
	else
		_root = root;
}

void Location_config::setFastCgi(std::string fastcgi_pass) {
	if (haveFastCgi())
		throw Location_config::FastCgiAlraedySetException();
	else
		_fastcgi_pass = fastcgi_pass;
}

void Location_config::setMethods(int methods) {
	if (methodsNull())
		throw Location_config::MethodsAlraedySetException();
	else
		_methods = methods;
}

const char *Location_config::RootAlraedySetException::what() const throw() {
	return ("EXCEPTION! Root Alraedy Set in this Location...");
};

const char *Location_config::FastCgiAlraedySetException::what() const throw() {
	return ("EXCEPTION! FastCgi Alraedy Set in this Location...");
};

const char *Location_config::MethodsAlraedySetException::what() const throw() {
	return ("EXCEPTION! Methods Alraedy Set in this Location...");
};