/*
 * Copyright (C) 2016 Aliaksei Verkhaturau
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "log2file.h"
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#endif



std::string pcType()
{
#ifdef _WIN32
    SYSTEM_POWER_STATUS powerStatus = {};
    GetSystemPowerStatus(&powerStatus);
    if (powerStatus.BatteryFlag == 128)
        return "desktop";
    else
        return "laptop";
#else
    return "unknown";
#endif
}

std::string osName()
{
#if defined(_WIN64)
    return "win64";  // 64-bit programs run only on Win64
#elif defined(_WIN32)
    // 32-bit programs run on both 32-bit and 64-bit Windows
    // so must sniff
    BOOL f64 = FALSE;
    return (IsWow64Process(GetCurrentProcess(), &f64) && f64 ? "win64" : "win32");
#else // not Windows
    return "not-Windows";
#endif
}

std::string osVer()
{
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4996)
    OSVERSIONINFO osvi = { sizeof(OSVERSIONINFO) };
    GetVersionEx(&osvi);
    return std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion);
#pragma warning(pop)
#else
    return std::string();
#endif
}


void Log2File::enable(bool makeEnabled)
{
    try {
        if (makeEnabled != logfile.is_open()) {
            if (makeEnabled) {
                openFile(filename);
                logfile << "Enabled logging...\n";
            } else {
                logfile << "Disabling logging...\n";
                logfile.close();
            }
        }
    } catch (...) {
        close();
    }
}

void Log2File::flush()
{
    logfile.flush();
}

void Log2File::close()
{
    logfile.close();
}

bool Log2File::isEnabled() const
{
    return logfile.is_open();
}

std::tr2::sys::path Log2File::logFilename() const
{
    return filename;
}

std::streamoff Log2File::currFileSize()
{
    return logfile.tellp().seekpos();
}

Log2File::Log2File(const std::tr2::sys::path& filename)
{
    openFile(filename);
}

void Log2File::openFile(const std::tr2::sys::path& filename)
{
    if (logfile.is_open() && this->filename == filename)
        return;
    this->filename = filename;
    std::error_code ec;
    if (!std::tr2::sys::exists(filename, ec))
        std::tr2::sys::create_directories(filename.parent_path(), ec);

    logfile.open(filename, std::ios::app);

    writeLogHeader();
}

Log2File::~Log2File()
{
    close();
}

void Log2File::writeLogHeader()
{
    if (logHeaderProvider)
        logfile << "========================================\n"
                << logHeaderProvider()
                << "\n========================================\n";
    else
        logfile << "========================================\n"
                << "Logger built " __TIMESTAMP__
                << "\nis running on " << pcType() << " powered by " << osName() << " " << osVer()
                << "\n========================================\n";
}
