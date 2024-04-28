#include "commandlineparser.h"
#include <QApplication>

CommandLineParser::CommandLineParser(QApplication &app)
    : m_target_directory_option{{"t", "target-directory"},
    QCoreApplication::translate("main", "Set default image(s) directory <directory>."),
    QCoreApplication::translate("main", "directory")} {
    addHelpOption();
    addVersionOption();

    addOption(m_target_directory_option);
    process(app);
}

QString CommandLineParser::path() const {
    auto list = values(m_target_directory_option);
    return list.isEmpty() ? "" : list.first();
}

