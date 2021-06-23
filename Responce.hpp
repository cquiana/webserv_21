#include "Webserv.hpp"

class Responce
{
private:

public:
    std::string _protocol;
    std::string _statusText;
    std::string _header;
    std::string _mimeType;
    int _status;
    size_t _lenght;

	std::string _result;

    Responce(/* args */);


    ~Responce();

    std::string getResponce();


};


