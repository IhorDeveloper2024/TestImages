#include "compression.h"
#include "../icompression.h"
#include "rawimagedata.h"
#include <QThread>

namespace {
static constexpr auto uchar_size = std::numeric_limits<unsigned char>::digits;
static constexpr auto step = 4;
static constexpr unsigned char white_color = 255;
QVector<bool> getIndexes(const CompressedData &input) {
    QVector<bool> result;
    for (const auto &item: input.indexes) {
        for (int index = uchar_size - 1; index >= 0; index--) {
            unsigned char mask = 1 << index;
            bool bit = item & mask;
            result.push_back(bit);
        }
    }
    return result;
}
} // unnames namespace

struct Compression::Impl {
    bool takeBit(const CompressedData &input) {
        const auto &character = input.numbers[m_byte_position];
        auto mask = 1 << m_bit_position;
        m_bit_position--;
        if (m_bit_position < 0) {
            m_bit_position = uchar_size - 1;
            m_byte_position++;
        }
        return character & mask;
    }

    unsigned char takeByte(const CompressedData &input) {
        unsigned char result = 0;
        for (int index = uchar_size - 1; index >= 0; index--) {
            auto mask = takeBit(input) << index;
            result |= mask;
        }
        return result;
    }

    void stop() { is_stop = true; }

    Result decompress(
        const CompressedData &input, QSharedPointer<ImageDataPtr> &output,
        std::function<void(int)> progress) {
        auto width = input.width;
        QVector<unsigned char> data;
        auto indexes = getIndexes(input);
        if (indexes[0]) {
            data.append(QVector<unsigned char>(input.width, white_color));
        }
        while(m_byte_position < input.numbers.size()) {
            if (is_stop) {
                return {};
            }
            if (progress) {
                progress(m_byte_position * 100 / input.numbers.size());
            }
            auto current_line = data.size() / input.width;
            if (current_line < indexes.size() - 1) {
                if (indexes[current_line + 1] && (data.size() % input.width == 0)) {
                    data.append(QVector<unsigned char>(input.width, 255));
                }
            }
            auto bit = takeBit(input);
            if (!bit) {
                auto chunk_size = width - data.size() % width > 4 ? 4 : width - data.size() % width;
                data.append(QVector<unsigned char>(chunk_size, 255));
            } else {
                bit = takeBit(input);
                if (!bit) {
                    auto chunk_size = width - data.size() % width > 4 ? 4 : width - data.size() % width;
                    data.append(QVector<unsigned char>(chunk_size, 0));
                } else { //gray
                    for(int index = 0; index < step; index++) {
                        auto byte = takeByte(input);
                        data.push_back(byte);
                        if (data.size() % width == 0) {
                            break;
                        }
                    }
                }
            }
        }
        output = QSharedPointer<ImageDataPtr>::create(
            input.width, input.height,  data
            );
        if (progress) { progress(101); }
        return {};
    }
private:
    bool is_stop = false;
    short m_bit_position = uchar_size - 1;
    int m_byte_position = 0;
};

Compression::Compression()
    : p_impl(QSharedPointer<Impl>::create()) {
}

Result Compression::decompress(
    const CompressedData &input, QSharedPointer<ImageDataPtr> &output,
    std::function<void(int)> progress) const {
    return p_impl->decompress(input, output, progress);
}

void Compression::stop() {
    p_impl->stop();
}
