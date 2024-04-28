#include "filemanager.h"
#include "icompression.h"
#include <QFile>

Result FileManager::load(const QString &name, CompressedData &output) {
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        return{" Cann't open file: " + name};
    }
    QDataStream in(&file);
    in >> output;
    file.close();
    if (output.height == 0 || output.width == 0 ||
        output.indexes_size != output.indexes.size() ||
        output.numbers_size != output.numbers.size()) {
        return {"Invalid data in file: " + name };
    }
    return {};
}

Result FileManager::save(const QString &name, const CompressedData &output) {
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly)) {
        return {" Cann't create file: " + name};
    }

    QDataStream out(&file);
    out << output;
    file.close();
    return {};
}
