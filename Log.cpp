#include "Log.h"
#include <iostream>
#include <cstdio>
#include <SDL.h> 

std::vector<LogEntry> Log::entries;
std::mutex Log::logMutex;

void Log::Add(LogType type, const char* fmt, ...)
{
    std::lock_guard<std::mutex> lock(logMutex);

    char buffer[2048];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    std::string msg(buffer);
    entries.push_back({ type, msg });


    switch (type)
    {
    case LogType::INFO:
        SDL_Log("%s", msg.c_str());
        break;
    case LogType::WARNING:
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
        break;
    case LogType::ERROR:
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", msg.c_str());
        break;
    default:
        std::cout << msg << std::endl;
        break;
    }
}

const std::vector<LogEntry>& Log::GetEntries()
{
    return entries;
}

void Log::Clear()
{
    std::lock_guard<std::mutex> lock(logMutex);
    entries.clear();
}