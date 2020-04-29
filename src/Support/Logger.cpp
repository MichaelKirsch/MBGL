

#include "Logger.h"

void MBGL::Logger::log(LogType type, std::string to_log,bool silent) {
    std::string message="";

    switch (type)
    {
        case (LogType::ERROR):
            message="ERROR:";
            break;

        case (LogType::FATAL_ERROR):
            message="FATAL_ERROR:";
            break;

        case (LogType::INFO):
            message="INFO:";
            break;

        case (LogType::WARNING):
            message="WARNING:";
            break;
    }

    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    message+= std::string(buf)+" | ";
    message+= to_log;

    m_loggedContent.emplace_back(message);
    if(!silent)
        std::cout << message << std::endl;
}

MBGL::Logger::~Logger() {
    std::cout << "*****PRINTING OUT LOGFILE*****" << std::endl;
    for(auto& i:m_loggedContent)
        std::cout<<i<<std::endl;
}
