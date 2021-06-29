//
// Created by Candy Quiana on 6/23/21.
//

#include "utils.hpp"


std::string NumberToString ( int num )
{
	std::ostringstream ss;
	ss << num;
	return ss.str();
}