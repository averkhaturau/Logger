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
    static std::string getLogfilenamePrefix();
    std::tr2::sys::path currentFilename()
    {
        static auto lfPrefix = getLogfilenamePrefix() + ("/" BRAND_COMPANYNAME "/" BRAND_NAME "/logfile-");
        return std::tr2::sys::path(lfPrefix + timestamp_filename() + ".txt");
    }
    std::function<bool()> rotationTrigger;
    Logger& m_logger;
public:
    LogRotator(Logger& logger, std::function<bool()> trigger)
        : m_logger(logger), rotationTrigger(trigger) {}

    void checkRotateLogfile();
};
