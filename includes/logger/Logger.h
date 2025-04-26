#ifndef SBERGAME_LOGGER_H
#define SBERGAME_LOGGER_H

#include <string>
#include <fstream>
#include <iostream>


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
    Logger(FileLogger* file_loggere);
    virtual ~Logger() {
        delete file_logger_;
    };

    void Log(const std::string &message) override;

private:
    FileLogger* file_logger_;
};

#endif //SBERGAME_LOGGER_H
