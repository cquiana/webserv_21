//
// Created by Elmo Barra on 6/18/21.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Server_config.hpp"
#include "Location_config.hpp"
#include "Http_config.hpp"

std::string ltrim(std::string in_string, const char* t = " \t\n\r\f\v")
{
	std::string ss = in_string;
	ss.erase(0, ss.find_first_not_of(t));
	return ss;
}

// trim from right
std::string rtrim(std::string in_string, const char* t = " \t\n\r\f\v")
{
	std::string ss = in_string;
	ss.erase(ss.find_last_not_of(t) + 1);
	return ss;
}

// trim from left & right
std::string wwtrim(std::string in_string, const char* t = " \t\n\r\f\v")
{
	size_t found_pattern = in_string.find(';');
	if (found_pattern == std::string::npos)
	{
		std::cout << "In " << in_string << " @not found `;`" << "\n";
		return ("");
	}
	else
		return ltrim(rtrim(in_string.substr(0, found_pattern), t), t);
}

size_t skipDigits(std::string in_string)
{
	size_t len = 0;
	std::string aaa = "0123456789\t\v\r\n ";

	for(std::string::iterator it = in_string.begin(); it != in_string.end(); it++) // ToDo WTF const !!!!?????
	{
		size_t ff = aaa.find_first_of((*it));
		if (ff != std::string::npos)
			return (len);
		len++;
	}
	return (-2);
}

std::string strString(std::string in_string, std::string search_string)
{
	return (wwtrim(in_string.substr(in_string.find(search_string) + search_string.length())));
}

int intString(std::string in_string, std::string search_string)
{
	return (atoi(in_string.substr(in_string.find(search_string) + search_string.length()).c_str()));
}

bool checkString(std::string in_string, std::string search_string)
{
	size_t found_pattern = in_string.find(search_string);
	size_t not_found_pattern = in_string.find_first_not_of("\t\v\r\n ");
	if (found_pattern == std::string::npos)
	{
		std::cout << "In " << in_string << " @not found " << search_string << "\n";
		return (false);
	}
	else if (not_found_pattern < found_pattern)
	{
		std::cout << "In " << in_string << " @error_0 in pattern " << search_string << "\n"; //ToDo DEL after debug
		return (false);
	}
	else
	{
		std::cout << "In " << in_string << " found " << search_string << " at pos: " << found_pattern << "\n"; //ToDo DEL after debug
		return (true);
	}
}

void parseConfig(std::string in_string2, Http_config* http_config)
{
	size_t found_pattern_hash = in_string2.find_first_of('#');
	std::string in_string = in_string2.substr(0, found_pattern_hash);
	if (in_string.length() < 3)
	{
		std::cout << "In " << in_string2 << " @ only #####" << "\n";
		return;
	}
	size_t e = in_string.find_first_of(';');
	if (checkString(in_string, "client_max_body_size"))
	{
		if (e != std::string::npos && !http_config->haveActiveServer())
			http_config->setMaxBody(intString(in_string, "client_max_body_size"));
		else
			std::cout << "In " << in_string << " @error in pattern __client_max_body_size" << "\n";
	}
	else if (checkString(in_string, "error_page"))
	{
		size_t ddd = in_string.find_first_of("0123456789");
		if (e != std::string::npos && !http_config->haveActiveServer())
			http_config->setErrorPage(intString(in_string, "error_page"), wwtrim(in_string.substr(ddd + skipDigits(in_string.substr(ddd)), e)));
		else
			std::cout << "In " << in_string << " @error in pattern __error_page" << "\n";
	}
	else if (checkString(in_string, "server"))
	{
		if (checkString(in_string, "server_name"))
		{
			if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
				http_config->_servers[http_config->getActiveServer()].setName(strString(in_string, "server_name"));
			else
				std::cout << "In " << in_string << " @error in pattern __server_name" << "\n";
		}
		else
		{
			size_t found_pattern2 = in_string.find("server");
			size_t found_pattern3 = in_string.find_first_of('{');
			size_t not_found_pattern = in_string.substr(0, found_pattern2).find_first_not_of("\t\v\r\n ");
			if (not_found_pattern == std::string::npos && found_pattern3 != std::string::npos &&
				http_config->haveActiveServer() == false)
				http_config->addServer();                                                                                    //ToDo need mark aktive server & location until its done
			else
				std::cout << "In " << in_string << " @error in pattern __server" << "\n";
		}
	}
	else if (checkString(in_string, "listen"))
	{
		if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setPort(intString(in_string, "listen"));
		else
			std::cout << "In " << in_string << " @error in pattern __listen" << "\n";
	}
	else if (checkString(in_string, "root"))
	{
		if (http_config->haveActiveServer() && http_config->_servers[http_config->getActiveServer()].haveActiveLocation() && e != std::string::npos)
			http_config->_servers[http_config->getActiveServer()]._locations[http_config->_servers[http_config->getActiveServer()].getActiveLocation()].setRoot(strString(in_string, "root"));
		else if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setRoot(strString(in_string, "root"));
		else
			std::cout << "In " << in_string << " @error in pattern __root" << "\n";
	}
	else if (checkString(in_string, "autoindex"))
	{
		size_t found_pattern4 = in_string.substr(0, e).find("on");
		size_t found_pattern5 = in_string.substr(0, e).find("off");
		if (http_config->haveActiveServer() && found_pattern4 != std::string::npos && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setAutoindex(1);
		else if (http_config->haveActiveServer() && found_pattern5 != std::string::npos && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setAutoindex(0);
		else
			std::cout << "In " << in_string << " @error in pattern __autoindex" << "\n";
	}
	else if (checkString(in_string, "index"))
	{
		if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setIndex(strString(in_string, "index"));
		else
			std::cout << "In " << in_string << " @error in pattern __index" << "\n";
	}
	else if (checkString(in_string, "location"))
	{
		size_t found_pattern = in_string.find("location") + 8;
		size_t found_pattern2 = in_string.find('~');
		size_t found_pattern3 = in_string.find_first_of('{');
		size_t not_found_pattern = in_string.substr(found_pattern).find_first_not_of("\t\v\r\n ");
		size_t found_pattern4 = in_string.find('.');
		size_t found_pattern5 = in_string.find('$');
		if (found_pattern2 != std::string::npos && not_found_pattern == std::string::npos && found_pattern3 != std::string::npos) // dl9 CGI
		{
			if (found_pattern4 != std::string::npos && found_pattern5 != std::string::npos)
				http_config->_servers[http_config->getActiveServer()].addLocation("", in_string.substr(found_pattern4 + 1, found_pattern5));
			else
				std::cout << "In " << in_string << " @error in CGI pattern __location" << "\n";
		}
		else if (found_pattern2 == std::string::npos && not_found_pattern == std::string::npos && found_pattern3 != std::string::npos) // dl9 PREFIX
			http_config->_servers[http_config->getActiveServer()].addLocation(strString(in_string, "location"), "");
		else
			std::cout << "In " << in_string << " @error in pattern __location" << "\n";
	}
	else if (checkString(in_string, "cgi_path"))
	{
		if (http_config->haveActiveServer() && http_config->_servers[http_config->getActiveServer()].haveActiveLocation() && e != std::string::npos)
			http_config->_servers[http_config->getActiveServer()]._locations[http_config->_servers[http_config->getActiveServer()].getActiveLocation()].setCgiPath(strString(in_string, "cgi_path"));
		else
			std::cout << "In " << in_string << " @error in pattern __cgi_path" << "\n";
	}
	else if (checkString(in_string, "methods"))
	{
		size_t found_pattern = in_string.find("GET POST DELETE");
		if (e != std::string::npos && found_pattern != std::string::npos && http_config->haveActiveServer() && http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()]._locations[http_config->_servers[http_config->getActiveServer()].getActiveLocation()].setMethods(1);
		else if (e != std::string::npos && found_pattern == std::string::npos && http_config->haveActiveServer() && http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()]._locations[http_config->_servers[http_config->getActiveServer()].getActiveLocation()].setMethods(0);
		else
			std::cout << "In " << in_string << " @error in pattern __cgi_path" << "\n";
	}
	else if (checkString(in_string, "return"))
	{
		size_t ddd = in_string.find_first_of("0123456789");
		if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setReturnCode(intString(in_string, "return"), wwtrim(in_string.substr(ddd + skipDigits(in_string.substr(ddd)), e)));
		else
			std::cout << "In " << in_string << " @error in pattern __return" << "\n";
	}
	else if (checkString(in_string, "}"))
	{
		if (http_config->haveActiveServer() && e == std::string::npos && http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].checkLastLocation();
		else if (http_config->haveActiveServer() && e == std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->checkLastServeer();
		else
			std::cout << "In " << in_string << " @error in close pattern `}`" << "\n";
	}
}

int main()
{
    std::fstream file_config;
    std::vector<std::string> vector_str_conf_file;
    std::string strbuf_config;
    Http_config http_config;
    int num_line = 0;

    if(!file_config.is_open())
        file_config.open("example.conf",std::ios::in); // proverit kak lu4she
    if (file_config.is_open()){   //checking whether the file is open
        while(getline(file_config, strbuf_config)){ //read data from file object and put it into string.

            std::size_t found_wrong_char = strbuf_config.find_first_not_of("abcdefghijklmnopqrstuvwxyz \t\n;:*#~()\\/.0123456789{}_|$ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            if (found_wrong_char != std::string::npos)
				std::cout << "Find wrong character ' " << strbuf_config[found_wrong_char] << " ' at position ' " << found_wrong_char << " ' in config file line " << num_line << std::endl << "\t" <<strbuf_config << std::endl << std::endl;
            std::replace( strbuf_config.begin(), strbuf_config.end(), '\t', '\0'); // replace all 'x' to 'y'
            if (strbuf_config.rfind("#", 0) != 0 && !strbuf_config.empty())
			{
				parseConfig(strbuf_config, &http_config);
				vector_str_conf_file.push_back(strbuf_config);
			}
//            vector_str_conf_file.push_back(parseConfig(strbuf_config));

//            std::cout << str_config << "\n"; //print the data of the string
			num_line++;
        }
        file_config.close(); //close the file object.
    }

    for (std::vector<std::string>::iterator it=vector_str_conf_file.begin(); it!=vector_str_conf_file.end(); ++it)
    {
        std::cout << *it << std::endl;
    }


    return 0;
}