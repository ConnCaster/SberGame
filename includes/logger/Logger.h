#ifndef SBERGAME_LOGGER_H
#define SBERGAME_LOGGER_H

#include <string>
#include <fstream>
#include <iostream>

enum class LogEventType : int {
    LOG_DEATHS = 0,
    LOG_SPEC_ACTS = 1
};

class ILogger {
public:
    virtual void Log(const std::string& message) {};
    virtual ~ILogger() = default;
};

class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename);
    ~FileLogger();

    void Log(const std::string &message) override;

private:
    std::string filename_;
    std::ofstream file_;
};

// @brief Pattern Proxy
class Logger : public ILogger {
public:
    Logger(FileLogger* file_logger, LogEventType log_event_type);
    virtual ~Logger() {
        delete file_logger_;
    };

    void Log(const std::string &message) override;

private:
    FileLogger* file_logger_;
    LogEventType log_event_type_;
};

#endif //SBERGAME_LOGGER_H
