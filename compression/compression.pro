TEMPLATE = lib
TARGET = compression
QT += core
DESTDIR=$$PWD/../../build/lib

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.rcc
UI_DIR = $$DESTDIR/.ui

INCLUDEPATH += \
    $$PWD/../common

SOURCES += \
    $$PWD/internal/compression.cpp \
    internal/decompression.cpp \

HEADERS += \
    $$PWD/icompression.h \
    $$PWD/../common/rawimagedata.h \
    $$PWD/../common/result.h \
    $$PWD/internal/compression.h \
