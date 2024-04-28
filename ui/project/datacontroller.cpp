#include "datacontroller.h"

#include <QBitmap>
#include <functional>

#include "imagescreen.h"
#include "fileslist.h"
#include "rawimagedata.h"
#include "icompression.h"

namespace {
QString getFileName(const QFileInfo &info) {
    QString name = info.fileName();
    if (name.contains("._packed")) {
        name = name.replace("_packed.barch", ".bmp");
    } else if (name.contains(".bmp") || name.contains(".png")) {
        name = name.replace(".bmp", "_packed.barch");
        name = name.replace(".png", "_packed.barch");
    } else {
        name = "(copy) " + name;
    }
    return info.absolutePath() + "/" + name;
}
}

DataController::DataController(QObject *parent)
    : QObject{parent}
    , m_fileslist(new FilesList()) {
    connect(static_cast<FilesList*>(m_fileslist), &FilesList::countChanged,
            this,  &DataController::filesListChanged);
    connect(static_cast<FilesList*>(m_fileslist), &FilesList::pathChanged,
            this,  &DataController::pathChanged);
}

DataController* DataController::Instance() {
    static DataController controller;
    return &controller;
}

QSharedPointer<ImageDataPtr> DataController::getImage() const {
    const auto &index = m_fileslist->index(m_currentIndex);
    static const auto emptyItem = QSharedPointer<ImageDataPtr>::create();
    if (!index.isValid()) {
        return emptyItem;
    }
    const auto &data = index.data(FilesList::ImageDataRole);
    if (!data.isValid()) {
        return {};
    }
    const auto &info = data.value<QSharedPointer<ImageDataPtr>>();
    if (info.isNull()) {
        return {};
    }
    return info;
}

void DataController::setIsVisible(bool visible) {
    if (m_isVisible == visible) {
        return;
    }
    m_isVisible = visible;
    emit isVisibleChanged();
}

void DataController::setSourceComponent(const QString &source) {
    m_sourceComponent = source;
    emit sourceChanged();
}

void DataController::select(const QString &screen) {
    static const QMap<QString, std::function<void()>> smap{
        {"QML", [this](){
            if (m_widget) {
                m_widget->deleteLater();
            }
            setSourceComponent("startScreen2");
            setIsVisible(true);
        }},
        {"QtWidget", [this](){
            DataController::setIsVisible(false);
            if (!m_widget) {
                m_widget = new ImageScreen();
            }
            m_widget->show();
        }},
        {"previous", [this](){
             if (m_widget) {
                 m_widget->deleteLater();
             }
             setSourceComponent("startScreen");
             setIsVisible(true);
        }},
        {"exit", [](){ exit(0); }}
    };
    if (!smap.contains(screen)) {
        return;
    }
    smap[screen]();
}

void DataController::navigate(int index) {
    static_cast<FilesList*>(m_fileslist)->navigate(index);
    selectImage(index);
    m_currentIndex = 0;
    emit indexChanged();
}

void DataController::selectImage(int index) {
    if (index < 0 || index >= m_fileslist->rowCount()) {
        return;
    }
    if (m_currentIndex == index) {
        return;
    }
    m_currentIndex = index;
    emit indexChanged();
}

void DataController::updateDirectoryPath(const QString &path) {
    static_cast<FilesList*>(m_fileslist)->updateDirectoryPath(path);
    emit pathChanged();
}

void DataController::setDefaultImageFolderPath(const QString &path) {
    static_cast<FilesList*>(m_fileslist)->setDefaultImageFolderPath(path);
    emit pathChanged();
}

QString DataController::getPath() const {
    return static_cast<FilesList*>(m_fileslist)->getPath();
}

QString DataController::save() {
    const auto &index = m_fileslist->index(m_currentIndex);
    const auto &data = index.data(FilesList::ImageDataRole);
    if (!data.isValid()) {
        return {"Not valid file."};
    }
    const auto &is_dir = index.data(FilesList::IsDirRole);
    if (is_dir.toBool()) {
        return {"It is directory."};
    }
    const auto &info = data.value<QSharedPointer<ImageDataPtr>>();
    if (info.isNull() || !info->isValid()) {
        return {"File data not valid."};
    }
    CompressedData output;
    auto result = ICompression::makeInstance()->compress(info->rowData(), output);
    if (result) {
        const auto &image_info = index.data(FilesList::FileInfoRole).value<QFileInfo>();
        result = m_filemanager.save(getFileName(image_info), output);
        if (result) {
            static_cast<FilesList*>(m_fileslist)->update();
        }
    }
    return result.message();
}
