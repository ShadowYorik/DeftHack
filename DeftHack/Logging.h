#pragma once

#include <iostream>


#define LOG_INFO(msg) std::cout << "[Info] " << msg << std::endl
#define LOG_WARNING_WITH_CODE(msg, errorCode) std::cout << "[Warning: " << errorCode << "] " << msg << std::endl
#define LOG_ERROR(msg) std::cerr << "[Error] " << msg << std::endl
#define LOG_ERROR_WITH_CODE(msg, errorCode) std::cerr << "[Error: " << errorCode << "] " << msg << std::endl
