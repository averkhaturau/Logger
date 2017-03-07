#include "logRotator.h"
#include "log-helpers.h"
#include "logger.h"


std::tr2::sys::path LogRotator::currentFilename()
{

    auto getLogfilenamePrefix = []() {
#ifdef _WIN32
        wchar_t logfileDir[MAX_PATH] = {};
        SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, reinterpret_cast<LPWSTR>(logfileDir));
        return std::tr2::sys::path(toUtf8(logfileDir)) / std::tr2::sys::path(BRAND_COMPANYNAME "/" BRAND_NAME "/logfile-");
#else
        return std::tr2::sys::path("logfile-");
#endif
    };
    static auto lfPrefix = getLogfilenamePrefix();
    return std::tr2::sys::path(lfPrefix).append(timestamp_filename() + ".txt");
}

void LogRotator::checkRotateLogfile()
{
    if (rotationTrigger && rotationTrigger()) {
        const bool isLoggerEnabled = m_logger.isEnabled();
        m_logger << "Starting new logfile";
        m_logger.enable(false);
        auto archFilename = m_logger.logFilename();
        m_logger.openFile(currentFilename());
        std::tr2::sys::rename(Logger::instance().logFilename(), archFilename, std::error_code());
        m_logger.enable(isLoggerEnabled);
        m_logger << "Continuing the logfile " << archFilename.string();
    }
}
