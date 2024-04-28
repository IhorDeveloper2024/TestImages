#pragma once

#include "result.h"
#include <QString>

struct CompressedData;
struct FileManager final {
    [[nodiscard]] Result load(const QString &name, CompressedData &output);
    [[nodiscard]] Result save(const QString &name, const CompressedData &output);
};
