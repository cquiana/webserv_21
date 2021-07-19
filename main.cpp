#include "Webserv.hpp"

//WebServer server;

//void sigHandler(int num) {
//	(void)num;
//	std::cout << "stoping webserv\n";
//	server.stop();
//	exit(1);
//}
//
//void catchSig(void){
//	signal(SIGINT, sigHandler);
//	signal(SIGQUIT, sigHandler);
//}

int main() {

	std::string conf = "example.conf";


	std::fstream file_config;
	//std::vector<std::string> vector_str_conf_file;
	std::string strbuf_config;
	Http_config http_config;
	int num_line = 0;

	//if(!file_config.is_open())
	//    file_config.open("example.conf",std::ios::in); // proverit kak lu4she
	try
	{
		file_config.open(conf,std::ios::in); // proverit kak lu4she
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
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return  1;
	}

	printConfig(&http_config);
	exit (1);

	WebServer webserver(&http_config);

	try {
//		if (server.init()) // conf file
//		if (webserver.init(&http_config)
//			return 1;
		// signal
		//catchSig();
		if (webserver.loop())
			return  1;
		} catch (const std::exception& e) {
			webserver.stop();
			return 1;
		}
	return 0;
}
