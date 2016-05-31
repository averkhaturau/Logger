import qbs

Group {
    name: "logger"
    prefix: name + '/'
    files: [
        "logger.h",
        "log2file.cpp",
        "log2file.h",
        "log-helpers.cpp",
        "log-helpers.h"
    ]

//    cpp.defines: ['BRAND_COMPANYNAME="YourCompany"', 'BRAND_NAME="YouApp"']
}
