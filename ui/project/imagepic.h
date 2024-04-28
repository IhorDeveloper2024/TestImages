#pragma once

#include <QQuickImageProvider>

class ImagePic : public QQuickImageProvider {
    Q_OBJECT
    QML_ELEMENT
public:
    ImagePic();

    QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize) override;
};
