#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <iostream>
#include <string>  // string
#include <iomanip> // std::setw, put_time

#include <ctime>   // localtime
#include <chrono>  // chrono::system_clock
#include <sstream> // stringstream

class Logger
{
public:
    Logger(std::string name, bool debug) :m_Name(name), m_Debug(debug) {};
    Logger(std::string name) :m_Name(name) {};
    ~Logger(){};

    template <typename T> void LogError(const T& t)
    {
        std::clog << getTimeStamp() << ": (" << std::setw(16) << std::left << m_Name << ") [ERROR] " << t << std::endl;
    }

    template <typename T> void LogWarn(const T& t)
    {
        std::clog << getTimeStamp() << ": (" << std::setw(16) << std::left << m_Name << ") [WARN ] " << t << std::endl;
    }

    template <typename T> void LogInfo(const T& t)
    {
        std::clog << getTimeStamp() << ": (" << std::setw(16) << std::left << m_Name << ") [INFO ] " << t << std::endl;
    }

    template <typename T> void LogDebug(const T& t)
    {
        if (m_Debug)
        {
            std::clog << getTimeStamp() << ": (" << std::setw(16) << std::left << m_Name << ") [DEBUG] " << t << std::endl;
        }
    }

    template <typename First, typename... Rest> void LogError(const First& first, const Rest&... rest)
    {
        std::clog << getTimeStamp() << ": (" << std::setw(16) << std::left << m_Name << ") [ERROR] " << first;
        Finish(rest...); // recursive call using pack expansion syntax
    }

    template <typename First, typename... Rest> void LogWarn(const First& first, const Rest&... rest)
    {
        std::clog << getTimeStamp() << ": (" << std::setw(16) << std::left << m_Name << ") [WARN ] " << first;
        Finish(rest...); // recursive call using pack expansion syntax
    }

    template <typename First, typename... Rest> void LogInfo(const First& first, const Rest&... rest)
    {
        std::clog << getTimeStamp() << ": (" << std::setw(16) << std::left << m_Name << ") [INFO ] " << first;
        Finish(rest...); // recursive call using pack expansion syntax
    }

    template <typename First, typename... Rest> void LogDebug(const First& first, const Rest&... rest)
    {
        if (m_Debug)
        {
            std::clog << getTimeStamp() << ": (" << std::setw(16) << std::left << m_Name << ") [DEBUG] " << first;
            Finish(rest...); // recursive call using pack expansion syntax
        }
    }

private:
    std::string m_Name;
    bool m_Debug = false;

    template <typename T> void Finish(const T& t)
    {
        std::clog << t << std::endl;
    }

    template <typename First, typename... Rest> void Finish(const First& first, const Rest&... rest)
    {
        std::clog << first;
        Finish(rest...); // recursive call using pack expansion syntax
    }

    std::string getTimeStamp()
    {
        std::stringstream ss;

        auto p = std::chrono::high_resolution_clock::now();
        //auto now_c = std::chrono::high_resolution_clock::to_time_t(p);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());
        auto s = std::chrono::duration_cast<std::chrono::seconds>(ms);
        std::time_t t = s.count();
        std::size_t fractional_seconds = ms.count() % 1000;

        //ss << std::put_time(std::localtime(&now_c), "%c"); ///gotta wait for c++14 support in gcc for this one...sigh
        char buf[20];
        std::strftime(buf, sizeof(buf), "%F %T", std::localtime(&t));
        ss << buf;
        ss << ".";
        ss << std::right << std::setw(3) << std::setfill('0') << fractional_seconds;
        return ss.str();
    }
};

#endif
