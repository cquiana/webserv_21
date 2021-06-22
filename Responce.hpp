#include "Webserv.hpp"

class Responce
{
private:

public:
    std::string _protocol;
    std::string _statusText;
    std::string _header;
    std::string _mymeType;
    int _status;
    unsigned int _lenght;

    Responce(/* args */);


    ~Responce();


};


