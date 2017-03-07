#pragma once

#include <filesystem>
#include <functional>
#include "log-helpers.h"

class Logger;

// Usage example:
//static struct MyLogRotator : public LogRotator {
//  MyLogRotator() : LogRotator(Logger::instance(), []() {
//      static tm lastRotation{};
//      time_t t; time(&t);
//      tm today; gmtime_s(&today, &t);
//      bool dayChanged = today.tm_mday != lastRotation.tm_mday;
//      lastRotation = today;
//      return dayChanged;
//  }) {
//      static auto me = this;
//      SetTimer(0, 0, 10000, static_cast<TIMERPROC>([](HWND, UINT, UINT_PTR, DWORD) {me->checkRotateLogfile(); }));
//  }
//} myLogRotator;

class LogRotator
{
protected:
    static std::string getLogfilenamePrefix();
    std::tr2::sys::path currentFilename();
    std::string m_moduleName;
    std::function<bool()> rotationTrigger;
    Logger& m_logger;
public:
    LogRotator(Logger& logger, std::function<bool()> trigger, std::string const& moduleName)
        : m_logger(logger), rotationTrigger(trigger), m_moduleName(moduleName) {}

    void checkRotateLogfile();
};
