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

void printConfig(Http_config* http_config)
{
	std::cout << "\nFull http_config:\n";
	std::cout << "client_max_body_size " << http_config->getMaxBody() << ";\n";

	for(std::vector<Server_config>::iterator it3 = http_config->_servers.begin(); it3 != http_config->_servers.end(); it3++)
	{
		std::cout << "server {\n";
		std::cout << "\tlisten " << (*it3).getPort() << ";\n";
		std::cout << "\tserver_name " << (*it3).getName() << ";\n";
		if ((*it3).blockedReturnCode())
		{
			if ((*it3).haveIndex())
				std::cout << "\tindex " << (*it3).getIndex() << ";\n";
			if ((*it3).haveAutoindex())
				std::cout << "\tautoindex " << ((*it3).getAutoindex() == 0 ? "off" : "on") << ";\n";
			if ((*it3).haveRoot())
				std::cout << "\troot " << (*it3).getRoot() << ";\n";


			std::vector<int>::iterator it1 = (*it3)._error_page_ints.begin();
			std::vector<std::string>::iterator it2 = (*it3)._error_page_strings.begin();
			while(it1 != (*it3)._error_page_ints.end() || it2 != (*it3)._error_page_strings.end())
			{
				std::cout << "\terror_page " << *it1 << " " << *it2 << ";\n";
				it1++;
				it2++;
			}

			for(std::vector<Location_config>::iterator it4 = (*it3)._locations.begin(); it4 != (*it3)._locations.end(); it4++)
			{
				if ((*it4).mIsCGI())
				{
					std::cout << "\tlocation ~ \\." << (*it4).getType() << "$ {\n";
					std::cout << "\t\tcgi_path " << (*it4).getCgiPath() << ";\n";
					std::cout << "\t}\n";
				}
				else if ((*it4).mIsPrefic())
				{
					std::cout << "\tlocation " << (*it4).getLocationPrefix() << " {\n";
					if ((*it4).haveRoot())
						std::cout << "\t\troot " << (*it4).getRoot() << ";\n";
					if ((*it4).haveMethods())
					{
						std::cout << "\t\tmethods GET";
						if ((*it4).getMethods() >= 6)
							std::cout << " POST";
						if ((*it4).getMethods() % 2 != 0)
							std::cout << " DELETE";
						std::cout << ";\n";
					}
					std::cout << "\t}\n";
				}
				else
					std::cout << "ERROR at PRINT_CONFIG\n";

			}
		}
		else
			std::cout << "\treturn " << (*it3).getReturnCode() << " " << (*it3).getReturnArdess() << ";\n";
		std::cout << "}\n";
	}
}

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
std::string wwtrim(std::string in_string, bool coma = true, const char* t = " \t\n\r\f\v")
{
	size_t found_pattern = in_string.find(';');
	if (found_pattern == std::string::npos && coma)
	{
		std::cout << "In " << in_string << " @not found `;`" << "\n";
		return ("");
	}
	else
		return ltrim(rtrim(in_string.substr(0, found_pattern), t), t);
}

size_t skipDigits(std::string in_string)
{
	std::string aaa = "0123456789\t\v\r\n ";

	size_t ff = in_string.find_first_not_of(aaa);
	if (ff != std::string::npos)
		return (ff);
	return (-2);

//	for(std::string::iterator it = in_string.begin(); it != in_string.end(); it++)
//	{
//		size_t ff = aaa.find_first_not_of((*it));
//		if (ff != std::string::npos)
//			return (len);
//		len++;
//	}
}

std::string strString(std::string in_string, std::string search_string, bool coma = true)
{
	return (wwtrim(in_string.substr(in_string.find(search_string) + search_string.length()), coma));
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
		//std::cout << "In " << in_string << " @not found " << search_string << "\n"; //ToDo enable for debug
		return (false);
	}
	else if (not_found_pattern < found_pattern)
	{
		std::cout << "In " << in_string << " @error_0 in pattern     " << search_string << ", #" << not_found_pattern <<  ", #" << found_pattern << "\n";	//ToDo fix only this after debug
		return (false);
	}
	else
	{
		//std::cout << "In " << in_string << " found " << search_string << " at pos: " << found_pattern << "\n"; //ToDo enable for debug
		return (true);
	}
}

void parseConfig(std::string in_string2, Http_config* http_config)
{
	size_t found_pattern_hash = in_string2.find_first_of('#');
	std::string in_string = in_string2.substr(0, found_pattern_hash);
	size_t found_pattern_skobka = in_string2.find_first_of('}');
	if ( in_string.length() < 3 && found_pattern_skobka == std::string::npos)
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
			std::cout << "ERROR!!! In " << in_string << " in pattern __client_max_body_size" << "\n";
	}
	else if (checkString(in_string, "error_page"))
	{
		size_t ddd = in_string.find_first_of("0123456789");
		if (e != std::string::npos && http_config->haveActiveServer() && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setErrorPage(intString(in_string, "error_page"), wwtrim(in_string.substr(ddd + skipDigits(in_string.substr(ddd)), e)));
		else
			std::cout << "ERROR!!! In " << in_string << " in pattern __error_page" << "\n";
	}
	else if (checkString(in_string, "server"))
	{
		if (checkString(in_string, "server_name"))
		{
			std::string sn = strString(in_string, "server_name");
			if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation() && !http_config->haveServer(sn))
				http_config->_servers[http_config->getActiveServer()].setName(sn);
			else
				std::cout << "ERROR!!! In " << in_string << " in pattern __server_name" << "\n";
		}
		else
		{
			size_t found_pattern2 = in_string.find("server");
			size_t found_pattern3 = in_string.find_first_of('{');
			size_t not_found_pattern = in_string.substr(0, found_pattern2).find_first_not_of("\t\v\r\n ");
			if (not_found_pattern == std::string::npos && found_pattern3 != std::string::npos) //&& http_config->haveActiveServer() == false)
				http_config->addServer();                                                                                    //ToDo need mark aktive server & location until its done
			else
				std::cout << "ERROR!!! In " << in_string << " in pattern __server" << "\n";
		}
	}
	else if (checkString(in_string, "listen"))
	{
		if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setPort(intString(in_string, "listen"));
		else
			std::cout << "ERROR!!! In " << in_string << " in pattern __listen" << "\n";
	}
	else if (checkString(in_string, "root"))
	{
		// std::cout << "In root # " << http_config->haveActiveServer() << " # " << http_config->_servers[http_config->getActiveServer()].haveActiveLocation() << " # " << strString(in_string, "root") << "\n";
		if (http_config->haveActiveServer() && e != std::string::npos && http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()]._locations[http_config->_servers[http_config->getActiveServer()].getActiveLocation()].setRoot(strString(in_string, "root"));
		else if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setRoot(strString(in_string, "root"));
		else
			std::cout << "ERROR!!! In " << in_string << " in pattern __root" << "\n";
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
			std::cout << "ERROR!!! In " << in_string << " in pattern __autoindex" << "\n";
	}
	else if (checkString(in_string, "index"))
	{
		if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setIndex(strString(in_string, "index"));
		else
			std::cout << "ERROR!!! In " << in_string << " in pattern __index" << "\n";
	}
	else if (checkString(in_string, "location"))
	{
		size_t found_pattern = in_string.find("location");// + 8;
		size_t found_pattern2 = in_string.find('~');
		size_t found_pattern3 = in_string.find_first_of('{');
		//std::cout << "location ## " << in_string.substr(0, found_pattern) << " ## \n";
		size_t not_found_pattern = in_string.substr(0, found_pattern).find_first_not_of("\t\v\r\n ");
		size_t found_pattern4 = in_string.find('.');
		size_t found_pattern5 = in_string.find('$');
		//std::cout << "location ### " << in_string << " " << found_pattern << " " << found_pattern2 << " " << found_pattern3 << " " << not_found_pattern << " " << found_pattern4 << " " << found_pattern5 << "\n";
		if (found_pattern2 != std::string::npos && not_found_pattern == std::string::npos && found_pattern3 != std::string::npos) // dl9 CGI
		{
			if (found_pattern4 != std::string::npos && found_pattern5 != std::string::npos)
				http_config->_servers[http_config->getActiveServer()].addLocation("", in_string.substr(found_pattern4 + 1, found_pattern5 - found_pattern4 - 1));
			else
				std::cout << "ERROR!!! In " << in_string << " in CGI pattern __location" << "\n";
		}
		else if (found_pattern2 == std::string::npos && not_found_pattern == std::string::npos && found_pattern3 != std::string::npos) // dl9 PREFIX
		{
			//std::cout << "addLocation " << in_string << " # " << in_string.substr(0, found_pattern3) << " # " << strString(in_string.substr(0, found_pattern3), "location", false) << "\n";
			http_config->_servers[http_config->getActiveServer()].addLocation(strString(in_string.substr(0, found_pattern3), "location",false), "");
		}
		else
			std::cout << "ERROR!!! In " << in_string << " in pattern __location" << "\n";
	}
	else if (checkString(in_string, "cgi_path"))
	{
		if (http_config->haveActiveServer() && e != std::string::npos && http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()]._locations[http_config->_servers[http_config->getActiveServer()].getActiveLocation()].setCgiPath(strString(in_string, "cgi_path"));
		else
			std::cout << "ERROR!!! In " << in_string << " in pattern __cgi_path" << "\n";
	}
	else if (checkString(in_string, "methods"))
	{
		int methods_int = 0; // DELETE = 1, POST = 2, GET = 4
		size_t found_pattern = in_string.find("GET");
		if (found_pattern != std::string::npos)
			methods_int += 4;
		size_t found_pattern2 = in_string.find("POST");
		if (found_pattern2 != std::string::npos)
			methods_int += 2;
		size_t found_pattern3 = in_string.find("DELETE");
		if (found_pattern3 != std::string::npos)
			methods_int += 1;
		if (e != std::string::npos && methods_int <= 7 && http_config->haveActiveServer() && http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()]._locations[http_config->_servers[http_config->getActiveServer()].getActiveLocation()].setMethods(methods_int);
		else
			std::cout << "ERROR!!! In " << in_string << " in pattern __cgi_path" << "\n";
	}
	else if (checkString(in_string, "return"))
	{
		size_t ddd = in_string.find_first_of("0123456789");
		if (http_config->haveActiveServer() && e != std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].setReturnCode(intString(in_string, "return"), wwtrim(in_string.substr(ddd + skipDigits(in_string.substr(ddd)), e)));
		else
			std::cout << "ERROR!!! In " << in_string << " in pattern __return" << "\n";
	}
	else if (checkString(in_string, "}"))
	{
		if (http_config->haveActiveServer() && e == std::string::npos && http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->_servers[http_config->getActiveServer()].checkLastLocation();
		else if (http_config->haveActiveServer() && e == std::string::npos && !http_config->_servers[http_config->getActiveServer()].haveActiveLocation())
			http_config->checkLastServeer();
		else
			std::cout << "ERROR!!! In " << in_string << " in close pattern `}`" << "\n";
	}
}

int ParseConfig2(std::string fileName)
{
    std::fstream file_config;
    //std::vector<std::string> vector_str_conf_file;
    std::string strbuf_config;
    Http_config http_config;
    int num_line = 0;

    //if(!file_config.is_open())
    //    file_config.open("example.conf",std::ios::in); // proverit kak lu4she
	file_config.open(fileName,std::ios::in); // proverit kak lu4she
    if (file_config.is_open()){   //checking whether the file is open
        while(getline(file_config, strbuf_config)){ //read data from file object and put it into string.

            std::size_t found_wrong_char = strbuf_config.find_first_not_of("abcdefghijklmnopqrstuvwxyz \t\n;:*#~()\\/.0123456789{}_-|$ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            if (found_wrong_char != std::string::npos)
				std::cout << "ERROR!!! Find wrong character ' " << strbuf_config[found_wrong_char] << " ' at position ' " << found_wrong_char << " ' in config file line " << num_line << std::endl << "\t" <<strbuf_config << std::endl << std::endl;
            std::replace( strbuf_config.begin(), strbuf_config.end(), '\t', '\0'); // replace all 'x' to 'y'
            if (strbuf_config.rfind('#', 0) != 0 && !strbuf_config.empty())
			{
				parseConfig(strbuf_config, &http_config);
				//vector_str_conf_file.push_back(strbuf_config);
			}
//            vector_str_conf_file.push_back(parseConfig(strbuf_config));

//            std::cout << str_config << "\n"; //print the data of the string
			num_line++;
        }
        file_config.close(); //close the file object.
    }

//	printConfig(&http_config);
//
//    for (std::vector<std::string>::iterator it=vector_str_conf_file.begin(); it!=vector_str_conf_file.end(); ++it)
//    {
//        std::cout << *it << std::endl;
//    }


    return 0;
}