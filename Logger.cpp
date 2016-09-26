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
		va_list args;
		auto fmt = format.c_str();
		va_start(args, fmt);
		__android_log_print(ANDROID_LOG_INFO, "Logger::log", format.c_str(), args );
		va_end(args);
		#endif
	}
}
