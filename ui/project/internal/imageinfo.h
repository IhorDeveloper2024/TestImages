#pragma once
#include "rawimagedata.h"
#include <QObject>
#include <QFileInfo>
#include <QThread>
#include <QSharedPointer>

class ICompression;
class ImageInfo final : public QObject {
    Q_OBJECT
public:
    ImageInfo(const QFileInfo &infos);
    ~ImageInfo();

    bool isValid() const;

    int getProgress() const { return m_progress; }
    void setProgress(int value);

    QFileInfo getFileInfo() const { return m_file_info; }
    QSharedPointer<ImageDataPtr> startLoadingImageData();
    QSharedPointer<ImageDataPtr> getImageDataPointer() const { return m_data_ptr; }
signals:
    void progressChanged();
    void isValidChanged();

private:
    QFileInfo m_file_info;
    QSharedPointer<ImageDataPtr> m_data_ptr;
    QThread m_thread;
    const int not_started = -1;
    const int file_loaded = 101;
    int m_progress = not_started;
    QSharedPointer<ICompression> m_compression;
};
