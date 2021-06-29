#include "Responce.hpp"

Responce::Responce(/* args */)
{
	_header = "";
	_statusText = "";
	_protocol = "HTTP/1.1";
	_statusText = "OK";
	_status = 200;
	_lenght = 0;
	_result = "";
}

Responce::~Responce()
{
}

std::string Responce::getResponce() {

}