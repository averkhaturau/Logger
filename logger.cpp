#include "logger.h"


#ifdef _WIN32
#include <Windows.h>
#include <Shlobj.h>
#endif

Logger& Logger::instance()
{
    auto getLogfilename = []() {
#ifdef _WIN32
        wchar_t logfileDir[MAX_PATH] = {};
        SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, reinterpret_cast<LPWSTR>(logfileDir));
        return std::tr2::sys::path(toUtf8(logfileDir)) / std::tr2::sys::path(BRAND_COMPANYNAME "/" BRAND_NAME "/logfile.txt");
#else
        return std::tr2::sys::path("logfile.txt");
#endif
    };
    static Logger l2f(getLogfilename());
    return l2f;
}

Logger::LogRecord::LogRecord(Logger& l2f, bool dub2console, std::mutex& m)
    : allowOnlyOneRecord(m), my_l2f(l2f), duplicateToConsole(dub2console)
{
    allowOnlyOneRecord.lock();
    my_l2f << timestamp(std::chrono::system_clock::now());
}

Logger::LogRecord::LogRecord(LogRecord&& a)
    : allowOnlyOneRecord(a.allowOnlyOneRecord), my_l2f(a.my_l2f), duplicateToConsole(std::move(a.duplicateToConsole))
{
    a.shouldFlush = false;
}

Logger::LogRecord::~LogRecord()
{
    if (shouldFlush) {
        // must put line break here to be thread-safe
        my_l2f << "\n"; my_l2f.flush();
        if (duplicateToConsole)
            std::cout << std::endl;
        allowOnlyOneRecord.unlock();
    }
}
