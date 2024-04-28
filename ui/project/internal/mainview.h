#pragma once

#include <QObject>

class QApplication;
class MainView final : public QObject {
    Q_OBJECT
public:
    using QObject::QObject;

    void start(QApplication &app, const QString &path);
signals:
    void screenCreationFailed();
};
