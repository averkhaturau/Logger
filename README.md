# C++ Logger

Usage:

In your repository:

git submodule add https://github.com/averkhaturau/Logger

In your project, add Logger directory to include directories.

E.g. in CMake, add_subdirectory(Logger),
also set Logger lib to your executable dependencies

in QBS, import "Logger/Logger.qbs" as logger;, and in your app "logger{}"


In your code:

// in your includes section
\#include \<logger.h\>

// in your code
Logger::instance() \<\< "log message";


Enjoy!

