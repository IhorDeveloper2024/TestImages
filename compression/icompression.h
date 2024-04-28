#pragma once

#include "result.h"
#include <QSharedPointer>
#include <QVector>

struct CompressedData {
    QVector<unsigned char> numbers;
    QVector<unsigned char> indexes;
    int width;
    int height;
    int numbers_size;
    int indexes_size;
};
QDataStream &operator>>(QDataStream &in, CompressedData &p);
QDataStream &operator<<(QDataStream &out, const CompressedData &p);


struct RawImageData;
struct ImageDataPtr;
struct Q_DECL_EXPORT ICompression {
    virtual ~ICompression() = default;
    [[nodiscard]] virtual Result compress(const RawImageData &input, CompressedData &output) const = 0;
    [[nodiscard]] virtual Result decompress(const CompressedData &input, QSharedPointer<ImageDataPtr> &output, std::function<void(int)> progress) const = 0;
    virtual void stop() = 0;

    static QSharedPointer<ICompression> makeInstance();
};
