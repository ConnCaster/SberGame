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
    virtual void Log(LogMessage* log_msg, LogType log_type) {};
    virtual ~ILogger() = default;
};

// @brief Pattern Observer
class LogMessage {
public:

    ~LogMessage() = default;

    void Attach(LogType log_type, ILogger* logger) {
        observed_loggers_[log_type] = logger;
        log_messages_[log_type] = {};
        log_idxs_[log_type] = 0;
    }

    void SetLogMessage(const std::string& log_msg, LogType log_type) {
        log_messages_[log_type].push_back(log_msg);
    }

    void ExecLog(LogType log_type, LogMode log_mode) {
        if (log_mode == LogMode::LOG_ALL) {
            NotifyLoggers(log_type, log_mode);
        } else {
            if (log_type == LogType::DEAD) {
                std::cout << "Do you want to save DEAD log messages [yes/no]: ";
                std::string is_need_log;
                do {
                    std::cin >> is_need_log;
                    if (is_need_log != "yes" && is_need_log != "no") {
                        std::cout << "Unknown enter. Choose again [yes/no]: " ;
                    }
                } while (is_need_log != "yes" && is_need_log != "no");
                if (is_need_log == "yes") {
                    NotifyLoggers(log_type, log_mode);
                } else {
                    log_idxs_[log_type] = 0;
                    log_messages_[log_type].clear();
                }
            } else if (log_type == LogType::SPEC_ACT) {
                std::cout << "Do you want to save SPEC_ACT log messages [yes/no]: ";
                std::string is_need_log;
                do {
                    std::cin >> is_need_log;
                    if (is_need_log != "yes" && is_need_log != "no") {
                        std::cout << "Unknown enter. Choose again [yes/no]: " ;
                    }
                } while (is_need_log != "yes" && is_need_log != "no");
                if (is_need_log == "yes") {
                    NotifyLoggers(log_type, log_mode);
                } else {
                    log_idxs_[log_type] = 0;
                    log_messages_[log_type].clear();
                }
            }
        }
    }

    std::string GetLogMsg(LogType log_type) {
        try {
            return log_messages_[log_type].at(log_idxs_[log_type]);
        } catch (...) {
            std::cout << "[ERROR] There is no log msg with idx" << std::endl;
            return "";
        }
    }

private:
    void NotifyLoggers(LogType log_type, LogMode log_mode) {
        if (log_mode != LogMode::LOG_ALL) {
            if (log_type == LogType::SPEC_ACT) {
                std::cout << "Do you want to save all messages in the turn (if no, every msg will be approved)[yes/no]: ";
                std::string is_need_log;
                do {
                    std::cin >> is_need_log;
                    if (is_need_log != "yes" && is_need_log != "no") {
                        std::cout << "Unknown enter. Choose again [yes/no]: " ;
                    }
                } while (is_need_log != "yes" && is_need_log != "no");
                if (is_need_log == "no") {
                    for (std::string& msg : log_messages_[log_type]) {
                        std::cout << msg << std::endl;
                        std::cout << "Do you want to save log message [yes/no]: ";
                        do {
                            std::cin >> is_need_log;
                            if (is_need_log != "yes" && is_need_log != "no") {
                                std::cout << "Unknown enter. Choose again [yes/no]: " ;
                            }
                        } while (is_need_log != "yes" && is_need_log != "no");
                        if (is_need_log == "no") {
                            log_idxs_[log_type]++;
                            continue;
                        }
                        observed_loggers_[log_type]->Log(this, log_type);
                        log_idxs_[log_type]++;
                    }
                    log_idxs_[log_type] = 0;
                    log_messages_[log_type].clear();
                    return;
                }
            }
        }
        for (std::string& msg : log_messages_[log_type]) {
            observed_loggers_[log_type]->Log(this, log_type);
            log_idxs_[log_type]++;
        }
        log_idxs_[log_type] = 0;
        log_messages_[log_type].clear();
    }

private:
    std::unordered_map<LogType, ILogger*> observed_loggers_{};
    std::unordered_map<LogType, std::vector<std::string>> log_messages_{};
    std::unordered_map<LogType, int> log_idxs_{};
};

class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename);

    ~FileLogger() {
        if (file_.is_open()) {
            file_.close();
        }
    }

    void Log(LogMessage* log_msg, LogType log_type) override;

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

    void Log(LogMessage* log_msg, LogType log_type) override {
        file_logger_->Log(log_msg, log_type);
    }

private:
    FileLogger* file_logger_;
};


class LogMsgHandler {
public:
    LogMsgHandler();

    void ResetLogger();

    void AddLogMsg(const std::string& msg, LogType log_type);
    void ExecLog(LogType log_type);
private:
    LogMessage log_msg_;
    LogMode log_mode_;
    std::vector<ILogger*> loggers_;
};

#endif //SBERGAME_LOGGER_H