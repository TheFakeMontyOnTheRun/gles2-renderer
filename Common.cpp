//
// Created by monty on 26/09/16.
//
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>
#include <vector>

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

std::vector<char> readToBuffer(FILE *fileDescriptor) {
	const unsigned N = 1024;
	std::vector<char> total;
	while (true) {
		char buffer[N];
		size_t read = fread((void *) &buffer[0], 1, N, fileDescriptor);
		if (read) {
			for (int c = 0; c < read; ++c) {
				total.push_back(buffer[c]);
			}
		}
		if (read < N) {
			break;
		}
	}

	return total;
}

std::string readToString(FILE *fileDescriptor) {

	std::string total;
	fseek(fileDescriptor, 0, SEEK_END);
	long fsize = ftell(fileDescriptor);
	fseek(fileDescriptor, 0, SEEK_SET);

	for ( auto pos = 0; pos < fsize; ++pos ) {
		char buffer[1];
		size_t read = fread((void *) &buffer[0], 1, 1, fileDescriptor);
		if (read) {
			for (int c = 0; c < read; ++c) {
				total.push_back(buffer[c]);
			}
		}
		if (read < 1) {
			return "total not met";
		}
	}

	return total;
}