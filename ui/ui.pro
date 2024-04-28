TARGET = compress_image_ui
DESTDIR=$$PWD/../../build
OBJECTS_DIR = $$DESTDIR/ui/.obj
MOC_DIR = $$DESTDIR/ui/.moc
RCC_DIR = $$DESTDIR/ui/.rcc
UI_DIR = $$DESTDIR/ui/.ui

QT += core gui qml quick quickwidgets concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += \
    $$PWD/../compression \
    $$PWD/../common \
    $$PWD/project/internal \
    $$PWD/project \

SOURCES += \
    $$PWD/../common/rawimagedata.cpp \
    $$PWD/project/commandlineparser.cpp \
    $$PWD/project/internal/imageinfo.cpp \
    $$PWD/project/internal/delegate.cpp \
    $$PWD/project/internal/filemanager.cpp \
    $$PWD/project/internal/fileslist.cpp \
    $$PWD/project/internal/mainview.cpp \
    $$PWD/project/datacontroller.cpp \
    $$PWD/project/imagepic.cpp \
    $$PWD/project/imagescreen.cpp \
    $$PWD/main.cpp


HEADERS += \
    $$PWD/../compression/icompression.h \
    $$PWD/../common/rawimagedata.h \
    $$PWD/../common/result.h \
    $$PWD/project/commandlineparser.h \
    $$PWD/project/datacontroller.h \
    $$PWD/project/imagepic.h \
    $$PWD/project/imagescreen.h \
    $$PWD/project/internal/imageinfo.h \
    $$PWD/project/internal/delegate.h \
    $$PWD/project/internal/filemanager.h \
    $$PWD/project/internal/fileslist.h \
    $$PWD/project/internal/mainview.h

LIBS += \
    -L$$PWD/../../build/lib \
    -lcompression

RESOURCES += \
    $$PWD/sources.qrc
