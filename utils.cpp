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

	if (stat(path.c_str(), &buff) != 0)
		return false;
	return true;
}

bool isDirectory(const std::string &path) {
	struct stat buff = {};

	if (stat(path.c_str(), &buff) != 0)
		return false;
	bool res = S_ISDIR(buff.st_mode);
	return res;
}

std::string ft_tolower(std::string str) {
	std::string result(str);
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

long checkSize(const char *str, int n) {
	long size;
	char *end = 0;
	size = std::strtol(str, &end, n);
	if (size == 0 && end == str)
		return -1;
	return size;
}

std::string ft_skip_space(std::string str)
{
	std::stringstream trimmer;
	trimmer << str;
	str.clear();
	trimmer >> str;
	return str;
}
