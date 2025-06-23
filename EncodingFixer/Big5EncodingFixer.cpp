// Big5EncodingFixer.cpp
#include "Big5EncodingFixer.h"
#include <algorithm>

size_t Big5EncodingFixer::fixEncoding(unsigned char* buffer, size_t length) {
    size_t i = 0;
    while (i + 1 < length) {
        if (buffer[i] >= 0x80) {
            std::swap(buffer[i], buffer[i + 1]);
            i += 2;
        } else {
            ++i;
        }
    }

    return (length - i);
}
