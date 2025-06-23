#pragma once
#include "IDataProvider.h"
#include <cstddef>
#include <sys/types.h>
#include <unistd.h>

class MmapDataProvider : public IDataProvider {
public:
    explicit MmapDataProvider(const char* filepath);
    virtual ~MmapDataProvider();

    size_t size() const override;
    bool mapChunk(size_t offset, size_t length, unsigned char*& buffer, size_t& actual_length) override;
    bool unmapChunk(unsigned char* buffer) override;

private:
    int m_fd;
    size_t m_file_size;
    const size_t m_page_size = sysconf(_SC_PAGE_SIZE);
};
