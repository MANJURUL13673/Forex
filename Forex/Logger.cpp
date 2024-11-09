#include "pch.h"
#include "Logger.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <mutex>

using namespace std;

// Mutex for thread safety
std::mutex logMutex;

// Constructor: Initializes the base filename and opens the initial log file
CLogger::CLogger(const std::string& baseFilename)
    : baseFilename(baseFilename)
{
    // Get current month and year
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    currentMonth = timeinfo->tm_mon + 1; // tm_mon is 0-11, so add 1 for 1-12
    currentYear = timeinfo->tm_year + 1900;

    openNewLogFile();
}

// Destructor: Closes the log file
CLogger::~CLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Opens a new log file based on the current date
void CLogger::openNewLogFile() {
    if (logFile.is_open()) {
        logFile.close();
    }

    string filename = generateFilename();
    logFile.open(filename, ios::app);
    if (!logFile.is_open()) {
        exit(0);
    }
}

// Generates a filename based on the current month and year
std::string CLogger::generateFilename() {
    ostringstream oss;
    oss << baseFilename << "_"
        << currentYear << "_"
        << setw(2) << setfill('0') << currentMonth
        << ".txt";
    return oss.str();
}

// Logs a message with a given log level
void CLogger::log(LogLevel level, const std::string& msg) {
    lock_guard<std::mutex> lock(logMutex);  // Ensure thread safety

    // Get current time information
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    int logMonth = timeinfo->tm_mon + 1;
    int logYear = timeinfo->tm_year + 1900;

    // Check if a new month or year has started
    if (logMonth != currentMonth || logYear != currentYear) {
        currentMonth = logMonth;
        currentYear = logYear;
        openNewLogFile();  // Open a new file for the new month
    }

    // Format the timestamp
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    // Create log entry
    ostringstream logEntry;
    logEntry << "[" << timestamp << "] "
        << levelToString(level) << ": " << msg << endl;

    // Output to console
    // cout << logEntry.str();

    // Output to log file
    if (logFile.is_open()) {
        logFile << logEntry.str();
        logFile.flush();  // Ensure immediate write to file
    }
}

// Converts log level to a string for output
std::string CLogger::levelToString(LogLevel level) {
    switch (level) {
    case DEBUG: return "DEBUG";
    case INFO: return "INFO";
    case WARNING: return "WARNING";
    case EXCEPTION: return "EXCEPTION";
    case CRITICAL: return "CRITICAL";
    default: return "UNKNOWN";
    }
}
