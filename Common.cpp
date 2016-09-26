//
// Created by monty on 26/09/16.
//
#include <string>
#include <iterator>
#include <sstream>

#include "Common.h"


float floatFrom(std::string str) {
	std::istringstream buffer(str);
	float temp;
	buffer >> temp;

	return temp;
}

int intFrom(std::string str) {
	std::istringstream buffer(str);
	int temp;
	buffer >> temp;

	return temp;
}

std::string filterComments(std::string input) {
	bool reading = true;
	std::stringstream output;

	for (auto &character : input) {

		if (character == '/') {
			reading = false;
		} else if (character == '\n') {
			reading = true;
		}

		if (reading) {
			output << character;
		}
	}

	return output.str();
}

