# C++ Logger

Usage:

In your repository:
'''
git submodule https://github.com/averkhaturau/Logger
'''

In your project, add Logger directory to include directories

In CMake, '''include_subdirectory(Logger)'''

In your code:
'''
// in your includes section
\#include \<logger.h\>

// in your code
Logger::instance() \<\< "log message";
'''

Enjoy!
