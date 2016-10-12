//
// Created by monty on 26/09/16.
//
#include <string>
#ifdef __ANDROID__
#include <android/log.h>
#endif

#include "Logger.h"
namespace odb {
	void Logger::log(std::string format, ...) {
		#ifdef __ANDROID__

		#endif
	}
}
