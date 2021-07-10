//
// Created by Candy Quiana on 6/23/21.
//

#pragma once
#include <sstream>
#include <sys/stat.h>
#include <cctype>
#include <time.h>

std::string NumberToString ( int num );
std::string cutLine(std::string &str, std::string const &elem);
std::string getTime(const time_t *time);
bool fileExist(const std::string &path);
bool isDirectory(const std::string &path);
std::string ft_tolower(std::string &str);
long checkSize(const char *str, int n);
std::string ft_skip_space(std::string str);






