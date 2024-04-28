#include "imageinfo.h"

#include "icompression.h"
#include "filemanager.h"
#include <QImage>
#include <QRunnable>
#include <QtConcurrent>
#include <QFutureWatcher>

ImageInfo::ImageInfo(const QFileInfo &info)
    : m_file_info(info)
    , m_compression(ICompression::makeInstance()){
    connect(&m_thread, &QThread::started, this, [this]() {
        m_data_ptr = startLoadingImageData();
    });
    moveToThread(&m_thread);
    m_thread.start();
}

ImageInfo::~ImageInfo() {
    m_compression->stop();
    m_thread.quit();
    m_thread.wait();
}

bool ImageInfo::isValid() const {
    return !m_data_ptr.isNull() && m_data_ptr->isValid();
}

void ImageInfo::setProgress(int value) {
    static const int delta_percent = 5;
    if (value == file_loaded || (m_progress == not_started)) {
        m_progress = value;
        emit isValidChanged();
        emit progressChanged();
    } else if ((value - m_progress) > delta_percent) {
        m_progress = value;
        emit progressChanged();
    }
}

QSharedPointer<ImageDataPtr> ImageInfo::startLoadingImageData() {
    QString path = m_file_info.absoluteFilePath();
    QString suffix = m_file_info.suffix();

    if (!QFile::exists(path)) {
        return {};
    }
    if (suffix == "bmp" || suffix == "png") {
        QImage image(path);
        image = image.convertToFormat(QImage::Format_Grayscale8);
        QByteArray arr = QByteArray::fromRawData((const char*)image.bits(), image.sizeInBytes());
        return QSharedPointer<ImageDataPtr>::create(
            image.width(), image.height(),
            QVector<unsigned char>{arr.constData(), arr.constData() + arr.size()}
        );
    } else {
        FileManager manager;
        CompressedData input;
        if (!manager.load(path, input)) {
            return {};
        }
        setProgress(0);
        QSharedPointer<ImageDataPtr> output;
        auto result = m_compression->decompress(input, output, [this](int value){
            setProgress(value);
        });
        if (!result) {
            qWarning() << "Image data is incorrect: " << result.message();
        }
        return result ? output : QSharedPointer<ImageDataPtr>();
    }
    return {};
}
