QT += qml

TEMPLATE = lib
CONFIG += qt plugin qmltypes

QML_IMPORT_NAME = FramesPlugin
QML_IMPORT_MAJOR_VERSION = 1

DESTDIR=../../build/qml_plugins/imports/$$QML_IMPORT_NAME

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.rcc
UI_DIR = $$DESTDIR/.ui

TARGET  = framesplugin

RESOURCES += FramesPlugin/plugin_qml.qrc

SOURCES += \
    src/FramesPlugin.cpp

HEADERS += \
    src/FramesPlugin.h

# copying dependecies of the plugin in the output-folder
PLUGIN_DEPENDENCY_DIRECTORY = $$PWD/FramesPlugin/Frames
PluginDependencies.commands = $(COPY_DIR) $$shell_path($$PLUGIN_DEPENDENCY_DIRECTORY) $$shell_path($$DESTDIR)

first.depends = $(first) PluginDependencies
export(first.depends)
export(PluginDependencies)
QMAKE_EXTRA_TARGETS += first PluginDependencies
