#pragma once
#include <QString>

struct Result final {
    Result(const QString &error = QString()) : m_message(error) {}
    operator bool() const {
        return m_message.isEmpty();
    }
    QString message() const { return m_message; }
private:
    QString m_message;
};
