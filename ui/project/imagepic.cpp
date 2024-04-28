#include "imagepic.h"
#include "datacontroller.h"
#include <QBitmap>
#include "rawimagedata.h"
#include "icompression.h"

ImagePic::ImagePic() : QQuickImageProvider(ImageType::Image) { }

QImage ImagePic::requestImage(const QString &id, QSize *size, const QSize& requestedSize) {
    Q_UNUSED(id)
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)
    auto image = DataController::Instance()->getImage();
    static QImage emptyImage(100, 100, QImage::Format_Grayscale8);
    emptyImage.fill(Qt::white);
    if (image.isNull() || !image->isValid()) {
        return emptyImage;
    }
    return {image->rowData().data, image->rowData().width,
            image->rowData().height - 5, QImage::Format_Grayscale8};
}
