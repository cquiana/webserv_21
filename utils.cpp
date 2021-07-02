//
// Created by Candy Quiana on 6/23/21.
//

#include "utils.hpp"


std::string NumberToString (int num) {
	std::ostringstream ss;
	ss << num;
	return ss.str();
}

std::string cutLine(std::string &str, std::string const &elem) {
	std::string res;

	res = str.substr(0, str.find(elem));
	if (!res.empty()) {
		str.erase(0, res.size() + elem.size());
	}
	return res;
}


std::string getTime(const time_t *time) {
	struct tm *timeinfo;
	char buff[128] = {};

	timeinfo = gmtime(time);
	strftime(buff, 128, "%a, %d %b %Y %Txs %Z", timeinfo);
	std::string res(buff);
	return res;
}

bool fileExist(const std::string &path) {
	struct stat buff = {};
	int ret;

	ret = stat(path.c_str(), &buff);
	if (ret != 0)
		return false;
	bool res = S_ISDIR(buff.st_mode);
	return res;
}

bool isDirectory(const std::string &path) {
	struct stat buff = {};
	int ret;

	ret = stat(path.c_str(), &buff);
	return (ret == 0);
}

