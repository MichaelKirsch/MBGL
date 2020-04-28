#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <ctime>

enum class LogType
{
    WARNING, ERROR, FATAL_ERROR, INFO
};

class Logger {
public:
    static Logger& get()
    {
        static Logger _instance;
        return _instance;
    }
    void log(LogType type, std::string to_log);
private:
    std::vector<std::string> m_loggedContent;
    Logger(const Logger&);
    Logger();
    Logger & operator = (const Logger&);
};



