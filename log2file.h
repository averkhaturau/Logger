#pragma once

/*
 * Copyright (C) 2016 Aliaksei Verkhaturau
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <string>
#include <fstream>
#include <filesystem>
#include <functional>
#include "log-helpers.h"

class Log2File
{
public:
    Log2File(const std::tr2::sys::path& filename);
    ~Log2File();

    template <class Arg_t>
    Log2File& operator<<(Arg_t&& mess)
    {
        if (isEnabled())
            logfile << toUtf8(mess);
        return *this;
    }

    void flush();
    void close();

    void enable(bool makeEnabled);
    bool isEnabled() const;

    std::tr2::sys::path logFilename() const;

    std::streamoff currFileSize();
    void openFile(const std::tr2::sys::path& filename);

    void setLogHeaderProvider(std::function<std::string()> fn) {logHeaderProvider = fn;};

private:
    Log2File(const Log2File&) = delete;
    Log2File(Log2File&&) = delete;
    Log2File& operator =(const Log2File&) = delete;

    std::tr2::sys::path filename;
    std::fstream logfile;

    void writeLogHeader();
    std::function<std::string()> logHeaderProvider;
};
