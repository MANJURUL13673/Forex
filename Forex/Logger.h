#pragma once
#include <fstream>
#include <string>
#include <ctime>

// Log level enum for categorizing log messages
enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    EXCEPTION,
    CRITICAL
};

class CLogger {
public:
    CLogger(const std::string& baseFilename);
    ~CLogger();
    void log(LogLevel level, const std::string& msg);

private:
    std::ofstream logFile;
    std::string baseFilename;
    int currentMonth;
    int currentYear;

    std::string generateFilename();
    void openNewLogFile();
    std::string levelToString(LogLevel level);
};
