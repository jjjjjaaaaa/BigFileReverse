#include "FileReverser.h"
#include <algorithm>

constexpr size_t CHUNK_SIZE = 8 * 1024 * 1024;  // 8 MB chunk size
#
bool FileReverser::process()
{
    size_t file_size = m_provider->size();
    
    size_t left = 0, right = file_size - 1;

    while (left < right)
    {
        size_t len = std::min(CHUNK_SIZE, (right - left + 1) / 2);

        unsigned char *left_buf = nullptr, *right_buf = nullptr;
        size_t left_len = 0, right_len = 0;

        if (!m_provider->mapChunk(left, len, left_buf, left_len))
        {
            return false;
        }
        if (!m_provider->mapChunk(right - len + 1, len, right_buf, right_len))
        {
            m_provider->unmapChunk(left_buf);
            return false;
        }

        for (size_t i = 0; i < len; ++i)
        {
            std::swap(left_buf[i], right_buf[len - 1 - i]);
        }

        m_provider->unmapChunk(left_buf);
        m_provider->unmapChunk(right_buf);

        left += len;
        right -= len;
    }

    size_t offset = 0;
    while (offset < file_size) {
    unsigned char* chunk = nullptr;
    size_t chunk_len = 0;

    size_t len = std::min(CHUNK_SIZE, file_size - offset);

    if (!m_provider->mapChunk(offset, len, chunk, chunk_len)) 
    {
        return false;
    }

    size_t remain = m_fixer->fixEncoding(chunk, chunk_len);
    m_provider->unmapChunk(chunk);

    size_t advance = (chunk_len > remain) ? (chunk_len - remain) : 1;
    offset += advance;
}

    return true;
}