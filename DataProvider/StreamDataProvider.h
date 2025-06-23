#pragma once
#include "IDataProvider.h"
#include <fstream>
#include <vector>

class StreamDataProvider : public IDataProvider {
public:
    explicit StreamDataProvider(const char* filepath);
    ~StreamDataProvider();

    size_t size() const override;
    bool mapChunk(size_t offset, size_t length, unsigned char*& buffer, size_t& actual_length) override;
    bool unmapChunk(unsigned char* buffer) override;

private:
    std::fstream m_file;
    size_t m_file_size;
    std::vector<std::vector<unsigned char>> m_buffer_pool;
};
