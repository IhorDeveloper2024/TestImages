#pragma once
#include <QCommandLineParser>

class QApplication;
class CommandLineParser : public QCommandLineParser {
public:
    CommandLineParser(QApplication &app);

    QString path() const;
private:
    QCommandLineOption m_target_directory_option;
};
