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

std::string ltrim(std::string s, const char* t = " \t\n\r\f\v")
{
	std::string ss = s;
	ss.erase(0, ss.find_first_not_of(t));
	return s;
}

// trim from right
std::string rtrim(std::string s, const char* t = " \t\n\r\f\v")
{
	std::string ss = s;
	ss.erase(ss.find_last_not_of(t) + 1);
	return s;
}

// trim from left & right
std::string wwtrim(std::string s, const char* t = " \t\n\r\f\v")
{
	return ltrim(rtrim(s, t), t);
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
		std::cout << "In " << in_string << " @error in pattern " << search_string << "\n"; //ToDo DEL after debug
		return (false);
	}
	else
	{
		std::cout << "In " << in_string << " found " << search_string << " at pos: " << found_pattern << "\n"; //ToDo DEL after debug
		return (true);
	}
}

void parseConfig(std::string in_string, Http_config* http_config)
{
	if (checkString(in_string, "client_max_body_size"))
	{
		http_config->setMaxBody(intString(in_string, "client_max_body_size"));
	}
	else if (checkString(in_string, "error_page"))
	{
		size_t i = intString(in_string, "error_page");
		size_t l = in_string.find_first_of("0123456789");
		size_t e = in_string.find_first_of(';');
		if (e != std::string::npos)
			http_config->setErrorPage(i, wwtrim(in_string.substr(l + skipDigits(in_string.substr(l)), e)));
		else
			std::cout << "In " << in_string << " @error in pattern error_page" << "\n";
	}
	else if (checkString(in_string, "server"))
	{
		size_t found_pattern = in_string.find("server");
		size_t found_pattern2 = in_string.find_first_of('#');
		size_t found_pattern3 = in_string.find_first_of('{');
		found_pattern += 6;
		size_t not_found_pattern = in_string.substr(found_pattern, found_pattern2).find_first_not_of("\t\v\r\n ;");
		if (not_found_pattern == std::string::npos && found_pattern3 != std::string::npos)
			http_config->addServer(); //ToDo need mark aktive server & location until its done



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