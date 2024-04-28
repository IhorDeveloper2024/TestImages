#pragma once

#include "../icompression.h"
#include "result.h"
#include <QtCore/QtGlobal>

class Compression final : public ICompression {
public:
    Compression();
    Result compress(const RawImageData &input, CompressedData &output) const override;
    Result decompress(const CompressedData &input, QSharedPointer<ImageDataPtr> &output, std::function<void(int)> progress) const override;
    void stop() override;
private:
    struct Impl;
    QSharedPointer<Impl> p_impl;
};
