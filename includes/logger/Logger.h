#ifndef SBERGAME_LOGGER_H
#define SBERGAME_LOGGER_H
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

enum class LogMode : int {
    LOG_ALL = 1,
    LOG_BY_REQ = 2
};

enum class LogType : int {
    DEAD = 1,
    SPEC_ACT = 2
};

class LogMessage;

class ILogger {
public:
    virtual void Log(LogMessage* log_msg) {};
    virtual ~ILogger() = default;
};

// @brief Pattern Observer
class LogMessage {
public:
    void Attach(LogType log_type, ILogger* logger) {
        observed_loggers[log_type] = logger;
    }

    void SetLogMessage(const std::string& log_msg, LogType log_type) {
        log_msg_ = log_msg;
        NotifyLoggers(log_type);
    }

    std::string GetLogMsg() const {
        return log_msg_;
    }

private:
    void NotifyLoggers(LogType log_type) {
        observed_loggers[log_type]->Log(this);
    }

private:
    std::unordered_map<LogType, ILogger*> observed_loggers;
    std::string log_msg_;
};

class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename);

    ~FileLogger() {
        if (file_.is_open()) {
            file_.close();
        }
    }

    void Log(LogMessage* log_msg) override;

private:
    std::string filename_;
    std::ofstream file_;
};

// @brief Pattern Proxy
class Logger : public ILogger {
public:
    Logger(FileLogger* file_logger)
            : file_logger_{file_logger}
    {}

    virtual ~Logger() {
        delete file_logger_;
    };

    void Log(LogMessage* log_msg) override {
        file_logger_->Log(log_msg);
    }

private:
    FileLogger* file_logger_;
};


class LogMsgHandler {
public:
    LogMsgHandler();

    void AddLogMsg(const std::string& msg, LogType log_type);

private:
    LogMessage log_msg_;
    LogMode log_mode_;
    std::vector<ILogger*> loggers_;
};

#endif //SBERGAME_LOGGER_H