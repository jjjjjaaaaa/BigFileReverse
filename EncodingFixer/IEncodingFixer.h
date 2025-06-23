#pragma once
#include <cstddef>

class IEncodingFixer {
public:
    virtual ~IEncodingFixer() = default;

    virtual size_t fixEncoding(unsigned char* buffer, size_t length) = 0;
};
