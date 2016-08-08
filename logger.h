#pragma once

/*
 * Copyright (C) 2016 Aliaksei Verkhaturau
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "log2file.h"
#include <filesystem>
#include <mutex>

#ifdef _WIN32
#include <Windows.h>
#include <Shlobj.h>
#endif

class Logger : public Log2File
{
public:
    static Logger& instance()
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

    class LogRecord
    {
    public:
        explicit LogRecord(Log2File& l2f)
            : my_l2f(l2f)
        {
            allowOnlyOneRecord.lock();
            my_l2f << timestamp(std::chrono::system_clock::now());
        }

        LogRecord(LogRecord&& a) : my_l2f(a.my_l2f) { a.shouldFlush = false; }
        template <class Arg_t>
        LogRecord&& operator<<(Arg_t&& mess)
        {
            my_l2f << mess;
            return std::move(*this);
        }
        ~LogRecord()
        {
            if (shouldFlush) {
                my_l2f << "\n"; my_l2f.flush();
                allowOnlyOneRecord.unlock();
            }
        }
    private:
        LogRecord() = delete;
        LogRecord(LogRecord const&) = delete;

        static std::mutex allowOnlyOneRecord;

        Log2File& my_l2f;
        bool shouldFlush = true;
    };

    template <class Arg_t>
    LogRecord operator<<(Arg_t&& mess)
    {
        return LogRecord(*this) << mess;
    }

private:
    Logger(std::tr2::sys::path const& fp) : Log2File(fp) {}
    Logger(Logger const&) = delete;
    Logger(Logger&&) = delete;
    Logger() = delete;
};

__declspec(selectany) std::mutex Logger::LogRecord::allowOnlyOneRecord;
