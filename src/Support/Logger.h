#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <ctime>

namespace MBGL
{
    enum LogType
    {
        WARNING, ERROR, FATAL_ERROR, INFO
    };

    class Logger {
    public:
        static Logger& get(){return _instance;};
        void log(MBGL::LogType type, std::string to_log, bool silent= true);
    private:
        std::vector<std::string> m_loggedContent;
        Logger(){};
        ~Logger();
        static Logger _instance;
    };
}



