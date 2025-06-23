#pragma once
#include "IEncodingFixer.h"

class Big5EncodingFixer : public IEncodingFixer {
public:
    size_t fixEncoding(unsigned char* buffer, size_t length) override;
};