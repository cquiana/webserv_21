//
// Created by Elmo Barra on 6/18/21.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

//std::string parseConfig(std::string in_string)
//{
//
//    if
//}

int main()
{
    std::fstream file_config;
    std::vector<std::string> vector_str_conf_file;
    std::string strbuf_config;

    if(!file_config.is_open())
        file_config.open("example.conf",std::ios::in); // proverit kak lu4she
    if (file_config.is_open()){   //checking whether the file is open
        while(getline(file_config, strbuf_config)){ //read data from file object and put it into string.

            std::size_t found_wrong_char = strbuf_config.find_first_not_of("abcdefghijklmnopqrstuvwxyz \t\n;*#~\\/.0123456789{}_|$ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            if (found_wrong_char != std::string::npos)
                std::cout << "Find wrong character ' " << strbuf_config[found_wrong_char] << " ' at position ' " << found_wrong_char << " ' in config file." << std::endl;
            std::replace( strbuf_config.begin(), strbuf_config.end(), '\t', '\0'); // replace all 'x' to 'y'
            if (strbuf_config.rfind("#", 0) != 0 && !strbuf_config.empty())
                vector_str_conf_file.push_back(strbuf_config);
//            vector_str_conf_file.push_back(parseConfig(strbuf_config));

//            std::cout << str_config << "\n"; //print the data of the string
        }
        file_config.close(); //close the file object.
    }

    for (std::vector<std::string>::iterator it=vector_str_conf_file.begin(); it!=vector_str_conf_file.end(); ++it)
    {
        std::cout << *it << std::endl;
    }


    return 0;
}