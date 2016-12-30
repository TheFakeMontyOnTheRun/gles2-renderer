//
// Created by monty on 26/09/16.
//
#include <string>
#include <iterator>
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
	const unsigned N = 1024;
	std::string total;
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