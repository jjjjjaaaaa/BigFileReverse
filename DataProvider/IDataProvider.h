#pragma once
#include <cstddef>

class IDataProvider {
public:
    virtual ~IDataProvider() = default;

    virtual size_t size() const = 0;

    virtual bool mapChunk(size_t offset, size_t length, unsigned char*& buffer, size_t& actual_length) = 0;

    virtual bool unmapChunk(unsigned char* buffer) = 0;
};
