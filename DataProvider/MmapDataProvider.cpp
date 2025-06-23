#include "MmapDataProvider.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstdio>
#include <algorithm>
#include <unordered_map>

struct MappedInfo
{
    void *base;
    size_t length;
};

static thread_local std::unordered_map<unsigned char *, MappedInfo> mmap_map;

MmapDataProvider::MmapDataProvider(const char *filepath)
    : m_fd(-1), m_file_size(-1)
{
    m_fd = open(filepath, O_RDWR);
    if (m_fd < 0)
    {
        perror("open");
        return;
    }

    struct stat st{};
    if (fstat(m_fd, &st) < 0)
    {
        perror("fstat");
        close(m_fd);
        m_fd = -1;
        return;
    }

    m_file_size = static_cast<size_t>(st.st_size);
}

MmapDataProvider::~MmapDataProvider()
{
    if (m_fd >= 0)
    {
        close(m_fd);
    }
}

size_t MmapDataProvider::size() const
{
    return m_file_size;
}

bool MmapDataProvider::mapChunk(size_t offset, size_t length, unsigned char*& buffer, size_t& actual_length)
{
    if (offset >= m_file_size)
    {
        return false;
    }

    off_t aligned = offset - (offset % m_page_size);
    size_t shift = offset - aligned;
    size_t map_len = std::min(m_file_size - aligned, shift + length);

    void* addr = mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, aligned);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        return false;
    }

    buffer = static_cast<unsigned char *>(addr) + shift;
    actual_length = std::min(length, m_file_size - offset);
    mmap_map[buffer] = {addr, map_len};
    return true;
}

bool MmapDataProvider::unmapChunk(unsigned char *buffer)
{
    auto it = mmap_map.find(buffer);
    if (it == mmap_map.end())
    {
        return false;
    }
    munmap(it->second.base, it->second.length);
    mmap_map.erase(it);
    return true;
}
