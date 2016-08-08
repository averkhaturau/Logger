#pragma once

/*
 * Copyright (C) 2016 Aliaksei Verkhaturau
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */


// To use the Logger as a default qDebug, qWarning, etc. logger, include this file and call
// qInstallMessageHandler(logAllTracing); from your int main()


#include <QString>
#include "logger.h"
#include <iostream>

void logAllTracing(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    static const char* const severity[] = {"[Debug]", "[Info]", "[Warning]", "[Critical]", "[Fatal]", "[System]"};
    // log into a file
    Logger::instance() << severity[type] << " " << msg.toStdWString();
    // log to debug console
    std::cout << severity[type] << ": " << msg.toStdString()
              << " (" << context.file << ":" << context.line
              << ", " << (context.function ? context.function : "") << ");\n";
}
