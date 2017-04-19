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
#include <iostream>


class Logger : public Log2File
{
public:
    static Logger& instance();

    class LogRecord
    {
    public:
        explicit LogRecord(Logger& l2f, bool dub2console, std::mutex& m);

        LogRecord(LogRecord&& a);

        template <class Arg_t>
        LogRecord&& operator<<(Arg_t const& mess)
        {
            my_l2f << mess;
            if (duplicateToConsole)
                std::cout << toUtf8(mess);
            return std::move(*this);
        }

        ~LogRecord();
    private:
        LogRecord() = delete;
        LogRecord(LogRecord const&) = delete;

        std::mutex& allowOnlyOneRecord;
        Log2File& my_l2f;
        bool shouldFlush = true;

        bool duplicateToConsole;
    };


    template <class Arg_t>
    LogRecord operator<<(Arg_t&& mess)
    {
        return LogRecord(*this, log2console, allowOnlyOneRecord) << mess;
    }

    class ForceToConsole {};
    template<>
    LogRecord operator<<(ForceToConsole&&) { return LogRecord(*this, true, allowOnlyOneRecord); }
    class LogfileOnly {};
    LogRecord operator<<(LogfileOnly&&) { return LogRecord(*this, false, allowOnlyOneRecord); }

    void setLogToConsole(bool b) {log2console = b;}
    bool logToConsole()const {return log2console;}

protected: // as you may need inheritance from Logger
    Logger(std::tr2::sys::path const& fp) : Log2File(fp) {}
    Logger(Logger const&) = delete;
    Logger(Logger&&) = delete;
    Logger() = delete;

    bool log2console = false;

    std::mutex allowOnlyOneRecord;
};
