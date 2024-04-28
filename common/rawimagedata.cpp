#include "rawimagedata.h"

ImageDataPtr::ImageDataPtr(int width, int height,  const QVector<unsigned char> &data)
    : m_vector(data)
    , m_data{width, height, m_vector.data()} {
    if (m_vector.size() == 0) {
        m_data.height = 0;
        m_data.width = 0;
    }
}

bool ImageDataPtr::isValid() const {
    return (m_data.height * m_data.width) != 0 &&
        m_data.width * m_data.height < m_vector.size();
}

const RawImageData& ImageDataPtr::rowData() const { return m_data; }

