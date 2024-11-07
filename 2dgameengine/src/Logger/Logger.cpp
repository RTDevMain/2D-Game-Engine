#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <ostream>
#include <sstream>


std::vector<LogEntry> Logger::messages;


static std::string getCurrentTime() {
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	std::tm localTime = *std::localtime(&time);

	std::ostringstream oss;
	oss << std::put_time(&localTime, "%m/%d/%Y %H:%M:%S");
	return oss.str();
}


void Logger::Log(const std::string& message){
	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.message = "Log: [" + getCurrentTime() + "]: " + message;
	std::cout << "\033[32m" << logEntry.message << "\033[0m" << std::endl;
	messages.push_back(logEntry);

}

void Logger::Err(const std::string& message){
	LogEntry logError;
	logError.type = LOG_ERROR;
	logError.message = "Err: [" + getCurrentTime() + "]: " + message;
	std::cerr << "\033[31m" << logError.message << "\033[0m" << std::endl;
	messages.push_back(logError);




}