#include "fileslist.h"
#include <QImage>

namespace {
QVector<QSharedPointer<ImageInfo>> getNewFileslist(const QString &m_path) {
    QVector<QSharedPointer<ImageInfo>> new_list;
    QDir directory{m_path};
    auto folders = directory.entryInfoList(QStringList());

    for (const auto &item: folders) {
        if (item.fileName() == "." || !item.isDir()) {
            continue;
        }
        new_list.push_back(QSharedPointer<ImageInfo>::create(item));
    }
    auto files = directory.entryInfoList({"*.bmp", "*.png", "*.barch"});
    for (const auto &item: files) {
        if (!item.isFile()) {
            continue;
        }
        new_list.push_back(QSharedPointer<ImageInfo>::create(item));
    }
    return new_list;
}
}


FilesList::~FilesList() {
}

void FilesList::navigate(int index) {
    if (index < 0 || index >= m_list.size()) {
        qWarning() << "navigate: Incorrect index";
        return;
    }
    const auto &item = m_list[index];
    if (item->getFileInfo().isDir()) {
        auto sub_path = item->getFileInfo().fileName();
        if (sub_path =="..") {
            QDir dir(m_path);
            auto result = dir.cdUp();
            if (!result) {
                return;
            }
            m_path = dir.absolutePath() + "/";
        } else if (!sub_path.isEmpty()) {
            m_path += sub_path + "/";
        } else {
            return;
        }
        update();
        emit pathChanged();
    }
}


void FilesList::updateDirectoryPath(const QString &path) {
    m_path = path + "/";
    update();
}

QHash<int, QByteArray> FilesList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IsDirRole] = "is_dir";
    roles[IsValidRole] = "is_valid";
    roles[FileInfoRole] = "fie_info";
    roles[FullDataRole] = "full_data";
    roles[ImageDataRole] = "image_data";
    roles[ProgressRole] = "progress";


    return roles;
}

void FilesList::setDefaultImageFolderPath(const QString &path) {
    if (path.isEmpty()) {
        m_path = QDir::currentPath() + "/";
    } else {
        QDir dir(path);
        if (!dir.exists()) {
            m_path = QDir::currentPath() + "/";
            qWarning() << "incorrect path: " << path;
            return;
        }
        m_path = path;
    }
    update();
    emit pathChanged();
}

void FilesList::update() {
    auto new_list = getNewFileslist(m_path);
    for (const auto&file: new_list) {
        connect(file.get(), &ImageInfo::progressChanged, this, [this]() {
            emit dataChanged(createIndex(0, 0), createIndex(m_list.size() - 1, 0));
        });
        connect(file.get(), &ImageInfo::isValidChanged, this, [this]() {
            emit isValidChanged();
        });
    }
    beginResetModel();
    m_list.clear();
    endResetModel();
    beginInsertRows(QModelIndex(), 0, new_list.size() - 1);
    m_list = new_list;
    endInsertRows();
}

int FilesList::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED(parent);
    return m_list.count();
}

QVariant FilesList::data( const QModelIndex& index, int role ) const {
    if (index.row() < 0 || index.row() >= m_list.count()) return QVariant();
    const auto &item = m_list[index.row()];
    if (role == NameRole) {
        if (item->getFileInfo().isFile()) {
            QString text("%1 (size: %2 bytes)");
            return text.arg(item->getFileInfo().fileName()).arg(item->getFileInfo().size());
        } else {
            return item->getFileInfo().fileName();
        }
    } else if (role == IsDirRole) {
        return item->getFileInfo().isDir();
    } else if (role == FileInfoRole) {
        return QVariant::fromValue(item->getFileInfo());
    } else if (role == IsValidRole) {
        return QVariant::fromValue(!item.isNull() && item->isValid());
    } else if (role == FullDataRole) {
        return QVariant::fromValue(item);
    } else if (role == ImageDataRole) {
        return QVariant::fromValue(item->getImageDataPointer());
    } else if (role == ProgressRole) {
        return item->getProgress();
    }

    return QVariant();
}
