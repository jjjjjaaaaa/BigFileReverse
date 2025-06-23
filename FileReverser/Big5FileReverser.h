#pragma once

#include "IFileReverser.h"
#include <memory>

class Big5FileReverser {
public:
    static std::shared_ptr<IFileReverser> MakeMmapReverser(const char* filepath);
    static std::shared_ptr<IFileReverser> MakeStreamReverser(const char* filepath);
};