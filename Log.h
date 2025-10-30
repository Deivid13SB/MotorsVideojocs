#pragma once
#include <string>
#include <cstdarg>
#include <vector>
#include <mutex>

enum class LogType
{
    INFO,
    WARNING,
    ERROR
};

struct LogEntry
{
    LogType type;
    std::string message;
};

class Log
{
public:
    static void Add(LogType type, const char* fmt, ...);
    static const std::vector<LogEntry>& GetEntries();
    static void Clear();

private:
    static std::vector<LogEntry> entries;
    static std::mutex logMutex;
};

#define LOG_INFO(fmt, ...)  Log::Add(LogType::INFO,    "[INFO] " fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  Log::Add(LogType::WARNING, "[WARN] " fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Log::Add(LogType::ERROR,   "[ERROR] " fmt, ##__VA_ARGS__)