#include "compression.h"
#include "../icompression.h"
#include "rawimagedata.h"
#include <QDataStream>

namespace {
static constexpr auto uchar_size = std::numeric_limits<unsigned char>::digits;
static constexpr auto step = 4;
static constexpr unsigned char white_color = 255;
static constexpr unsigned char black_color = 0;
static const auto test_black = QVector<unsigned char>(step, black_color);
enum Type { white = 0, black, gray };

Type getChunkType(const unsigned char *items, int pos,
                  const QVector<unsigned char> &test_white, int delta) {
    if (memcmp(items + pos, test_white.constData(), delta) == 0)  { return white; }
    if (memcmp(items + pos, test_black.constData(), delta) == 0)  { return black; }
    return gray;
}

QVector<unsigned char> getIndexes(const RawImageData &input,
                                  const QVector<unsigned char> &test_white) {
    QVector<unsigned char> resulted(
        input.height % uchar_size ? input.height / uchar_size + 1 :
            input.height / uchar_size);
    for (int index = 0; index < input.height; index++) {
        auto isWhite = memcmp(input.data + input.width * index,
                              test_white.constData(), input.width) == 0;
        resulted[index / uchar_size] |= (isWhite << (7 - (index % uchar_size)));
    }
    return resulted;
}

bool isWhiteLine(const QVector<unsigned char> &indexes, int row) {
    auto character = indexes[row / uchar_size];
    auto mask = 1 << (uchar_size - row % uchar_size - 1);
    return character & mask;
}

void decreaseBitPosition(short &bit_position, QVector<unsigned char> &result,
                         bool addChar = true) {
    bit_position--;
    if (bit_position < 0) {
        bit_position += uchar_size;
        if (addChar) {
            result.push_back(0);
        }
    }
}

void addGrayData(
    QVector<unsigned char> &result, int delta,
    const unsigned char *items, short &bit_position, int pos) {
    result.append(QVector<unsigned char>(delta, 0));
    auto new_delta = result.size() - delta - 1;
    for (int row = 0; row < delta; row++) {
        const auto &point_data = (items + pos)[row];
        for (int bit_pos = uchar_size - 1; bit_pos >= 0; bit_pos--) {
            result[new_delta] |= (static_cast<bool>(point_data & (1 << bit_pos)) << bit_position);
            if (bit_position == 0) {
                new_delta++;
            }
            decreaseBitPosition(bit_position, result, false);
        }
    }
}

QVector<unsigned char> compressLine(unsigned char point,
                                    const RawImageData &input, short &bit_position, int row) {
    QVector<unsigned char> result{point};
    const QVector<unsigned char> test_white(input.width, white_color);
    for (int pos = 0; pos < input.width; pos += step) {
        const auto &items = input.data + input.width * row;
        auto delta = input.width - pos < step ? input.width - pos : step;
        static const QHash<Type, std::function<void(short &, QVector<unsigned char>&)>> smap{
            {white, [](short &bit_position, QVector<unsigned char>&result){
                 decreaseBitPosition(bit_position, result);
             }},
            {black, [](short &bit_position, QVector<unsigned char>&result){
                 result.last() |= 1 << bit_position;
                 decreaseBitPosition(bit_position, result);
                 result.last() |= 0 << bit_position;
                 decreaseBitPosition(bit_position, result);
             }},
            {gray, [&delta, &pos, &items](short &bit_position, QVector<unsigned char>&result){
                 result.last() |= 1 << bit_position;
                 decreaseBitPosition(bit_position, result);
                 result.last() |= 1 << bit_position;
                 decreaseBitPosition(bit_position, result);
                 addGrayData(result, delta, items, bit_position, pos);
             }}
        };
        auto type = getChunkType(items, pos, test_white, delta);
        if (!smap.contains(type)) {
            qWarning() << "Incorect chunk type: " << type;
            continue;
        }
        smap[type](bit_position, result);
    }
    return result;
}

void addCompressedLine(const RawImageData &input, int index, CompressedData &output, short &bit_position) {
    output.numbers.append(compressLine(
        output.numbers.takeLast(), input, bit_position, index)
                          );
}
} // unnamed namespace

Result Compression::compress(const RawImageData &input, CompressedData &output) const {
    if (input.height * input.width == 0) {
        return {"Data is absent"};
    }
    const QVector<unsigned char> test_white(input.width, white_color);
    output.indexes = getIndexes(input, test_white);
    output.numbers.push_back(0);
    short bit_position = uchar_size - 1;
    for (int index = 0; index < input.height; index++) {
        if (isWhiteLine(output.indexes, index)) {
            continue;
        }
        addCompressedLine(input, index, output, bit_position);
    }
    output.width = input.width;
    output.height = input.height;
    return {};
}

QDataStream &operator>>(QDataStream &in, CompressedData &p){
    in >> p.width >> p.height >> p.indexes >>
          p.numbers >> p.numbers_size >> p.indexes_size;
    return in;
}

QDataStream &operator<<(QDataStream &out, const CompressedData &p){
    out << p.width << p.height << p.indexes << p.numbers
        << static_cast<int>(p.numbers.size())
        << static_cast<int>(p.indexes.size());
    return out;
}

QSharedPointer<ICompression> ICompression::makeInstance() {
    return QSharedPointer<Compression>::create();
}
