#pragma once

#include<QSharedPointer>
#include <QDebug>
#include <QVector>
#include <QIODevice>


struct RawImageData {
    int width; // image width in pixels
    int height; // image height in pixels
    unsigned char * data; // Pointer to image data. data[j * width + i] is color of pixel in row j and column i.
};

struct ImageDataPtr final {
    ImageDataPtr() = default;
    ImageDataPtr(int width, int height,  const QVector<unsigned char> &data);

    bool isValid() const;
    const RawImageData& rowData() const;
private:
    QVector<unsigned char> m_vector;
    RawImageData m_data = {0, 0, nullptr};
};
