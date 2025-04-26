#include "logger/Logger.h"

FileLogger::FileLogger(const std::string &filename)
    : filename_(filename)
{
   file_.open(filename_);
   if (!file_.is_open()) {
       std::cout << "Error open file in logger" << std::endl;
   }
}

void FileLogger::Log(const std::string &message) {
    if (file_.is_open()) {
        file_ << message;
        file_.flush();
    }
}

FileLogger::~FileLogger() {
    if (file_.is_open()) {
        file_.close();
    }
}

Logger::Logger(FileLogger* file_logger)
    : file_logger_{file_logger}
{}


void Logger::Log(const std::string &message) {
    file_logger_->Log(message);
}

