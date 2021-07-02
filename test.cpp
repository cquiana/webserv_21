#include "Webserv.hpp"
#include "utils.hpp"

int main()
{

	std::string res;
	std::string tmp;
	std::ifstream is("./request.txt");
	if (is.is_open()) {
		while (getline(is, tmp)) {
			res.append(tmp);
		}
	}
	// std::cout << "you are here\n";
	is.close();
	std::string str = cutLine(res, "*");
	std::cout << str;
	std::cout << std::endl;
	std::cout << res;


	return 0;
}
