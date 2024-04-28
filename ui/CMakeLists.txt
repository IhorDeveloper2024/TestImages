# set(CMAKE_INSTALL_PREFIX /home/igor/Qt/6.6.1/gcc_64/lib/cmake)

cmake_minimum_required(VERSION 3.16)

project(compress_image_ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(XKB_LIBRARY XKB_LIBRARY-NOTFOUND)
set(XKB_INCLUDE_DIR XKB_INCLUDE_DIR-NOTFOUND)
set(CMAKE_BUILD_TYPE:STRING Release)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY {PWD}/../build/compression)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Widgets Concurrent Qml Quick)


include_directories("../common")
include_directories("../compression")
include_directories("project")
include_directories("project/internal")

qt_add_executable(${PROJECT_NAME}
    ../common/rawimagedata.cpp 
    project/commandlineparser.cpp 
    project/internal/imageinfo.cpp 
    project/internal/delegate.cpp 
    project/internal/filemanager.cpp 
    project/internal/fileslist.cpp 
    project/internal/mainview.cpp 
    project/datacontroller.cpp 
    project/imagepic.cpp 
    project/imagescreen.cpp 
    main.cpp

    ../compression/icompression.h
    ../common/rawimagedata.h
    ../common/result.h
    project/commandlineparser.h
    project/datacontroller.h
    project/imagepic.h
    project/imagescreen.h
    project/internal/imageinfo.h
    project/internal/delegate.h
    project/internal/filemanager.h
    project/internal/fileslist.h
    project/internal/mainview.h
    sources.qrc
)

qt_add_resources(sources.qrc)

message(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../lib)

find_library(COMPRESSION compression HINTS /home/igor/TestPacketBook/TestImpl/build/lib/)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Quick
        ${COMPRESSION}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
