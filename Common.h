//
// Created by monty on 26/09/16.
//

#ifndef DUNGEONSOFNOUDAR_NDK_COMMON_H
#define DUNGEONSOFNOUDAR_NDK_COMMON_H

float floatFrom(std::string str);

int intFrom(std::string str);

std::string filterComments(std::string input);

std::string readToString(FILE *fileDescriptor);

std::vector<char> readToBuffer(FILE *fileDescriptor);
#endif //DUNGEONSOFNOUDAR_NDK_COMMON_H
