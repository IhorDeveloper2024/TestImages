#pragma once

#include "imageinfo.h"

#include <QAbstractListModel>
#include <QtQml>
#include <QString>
#include <QSharedPointer>

class FilesList final : public QAbstractListModel {
    Q_OBJECT
public:
    using QAbstractListModel::QAbstractListModel;
    ~FilesList();

    enum PersonRoles {
        NameRole = Qt::UserRole + 1,
        IsDirRole,
        IsValidRole,
        FileInfoRole,
        FullDataRole,
        ImageDataRole,
        ProgressRole
    };
    void navigate(int index);
    void update();
    void updateDirectoryPath(const QString &path);
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;

    void setDefaultImageFolderPath(const QString &path);

    QString getPath() const { return m_path; }

signals:
    void countChanged();
    void pathChanged();
    void isValidChanged();
private:
    QVector<QSharedPointer<ImageInfo>> m_list;
    QString m_path;
};
