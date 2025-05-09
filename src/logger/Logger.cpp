#include "logger/Logger.h"

// int LogMessage::dead_idx = 0;
// int LogMessage::spec_act_idx = 0;

FileLogger::FileLogger(const std::string &filename)
        : filename_(filename) {
    file_.open(filename_);
    if (!file_.is_open()) {
        std::cout << "Error open file in logger" << std::endl;
    }
}

void FileLogger::Log(LogMessage *log_msg, LogType type) {
    if (file_.is_open()) {
        file_ << log_msg->GetLogMsg(type);
        file_.flush();
    }
}

LogMsgHandler::LogMsgHandler()
        : log_msg_{} {
    std::cout << "Choose logger mode [all=1 / by_request=2]: ";
    std::string type;
    do {
        std::cin >> type;
        if (type != "1" && type != "2") {
            std::cout << "unknown logger mode. Choose again [all=1 / by_request=2]: ";
        }
    } while (type != "1" && type != "2");
    log_mode_ = static_cast<LogMode>(std::stol(type));

    loggers_.push_back(new FileLogger("./logger_spec_acts.log"));
    loggers_.push_back(new FileLogger("./logger_deaths.log"));

    log_msg_.Attach(LogType::SPEC_ACT, loggers_[0]);
    log_msg_.Attach(LogType::DEAD, loggers_[1]);
}

void LogMsgHandler::AddLogMsg(const std::string& msg, LogType log_type) {
        log_msg_.SetLogMessage(msg, log_type);
}

void LogMsgHandler::ExecLog(LogType log_type) {
    log_msg_.ExecLog(log_type, log_mode_);
}