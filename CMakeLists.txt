project(Logger)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SRC
	logger.h
	log2file.cpp
	log2file.h
	log-helpers.cpp
	log-helpers.h
)

#add_definitions("-DBRAND_COMPANYNAME=\"CompanyName\" -DBRAND_NAME=\"BrandName\"")

add_library(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "libs")