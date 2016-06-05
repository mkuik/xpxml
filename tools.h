/*
 * xConvertString.h
 *
 *  Created on: Nov 19, 2014
 *      Author: matthijs
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <sstream>
#include <string>
#include <inttypes.h>
#include <vector>

typedef std::vector<std::string>::iterator param_t;

template<typename T>
T convert(const std::string &data) {
	if (!data.empty()) {
		T ret;
		std::istringstream iss(data);
		if (data.find("0x") != std::string::npos) {
			iss >> std::hex >> ret;
		} else {
			iss >> std::dec >> ret;
		}

		if (iss.fail()) {
			throw std::exception();
		}
		return ret;
	}
	throw std::exception();
}

template<typename T>
std::string toString(T Number) {
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}




#endif /* TOOLS_H_ */
