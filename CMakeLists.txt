project(Logger)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SRC
	logger.h
	logger.cpp
	log2file.cpp
	log2file.h
	log-helpers.cpp
	log-helpers.h
	logRotator.h
	logRotator.cpp
)

#add_definitions("-DBRAND_COMPANYNAME=\"CompanyName\" -DBRAND_NAME=\"BrandName\"")

add_library(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "libs")