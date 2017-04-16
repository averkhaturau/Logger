#include "logRotator.h"
#include "log-helpers.h"
#include "logger.h"

#ifdef _WIN32
#include <Windows.h>
#include <Shlobj.h>
#endif


std::string LogRotator::getLogfilenamePrefix()
{
#ifdef _WIN32
    wchar_t logfileDir[MAX_PATH] = {};
    SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, reinterpret_cast<LPWSTR>(logfileDir));
    return toUtf8(logfileDir);
#else
    return ".";
#endif
}

std::tr2::sys::path LogRotator::currentFilename()
{
    static auto lfPrefix = getLogfilenamePrefix() + ("/" BRAND_COMPANYNAME "/") + m_moduleName + "/logfile-";
    return std::tr2::sys::path(lfPrefix + timestamp_filename().substr(0, 10) + ".txt");
}

void LogRotator::checkRotateLogfile()
{
    if (rotationTrigger && rotationTrigger()) {
        const bool isLoggerEnabled = m_logger.isEnabled();
        m_logger << "Starting new logfile";
        m_logger.enable(false);
        auto archFilename = m_logger.logFilename();
        m_logger.openFile(currentFilename());
        //std::tr2::sys::rename(Logger::instance().logFilename(), archFilename, std::error_code());
        m_logger.enable(isLoggerEnabled);
        m_logger << "Continuing the logfile " << archFilename.string();
    }
}

