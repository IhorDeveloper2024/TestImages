cmake_minimum_required(VERSION 3.16)

project(compression VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(XKB_LIBRARY XKB_LIBRARY-NOTFOUND)
set(XKB_INCLUDE_DIR XKB_INCLUDE_DIR-NOTFOUND)
set(CMAKE_BUILD_TYPE:STRING Release)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core)

include_directories("../common")

add_library(${PROJECT_NAME} SHARED #NOT_SHARED
	icompression.h
	internal/compression.h
	internal/compression.cpp
	internal/decompression.cpp
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
)
