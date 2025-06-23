#include "StreamDataProvider.h"
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include <unordered_map>

struct ChunkInfo
{
    unsigned char *data;
    size_t length;
    size_t offset_in_file;
};
static thread_local std::unordered_map<unsigned char *, ChunkInfo> chunk_map;

StreamDataProvider::StreamDataProvider(const char *filepath)
    : m_file_size(-1)
{
    m_file.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
    if (!m_file)
    {
        perror("open (stream)");
        return;
    }

    struct stat st{};
    if (stat(filepath, &st) == 0)
    {
        m_file_size = static_cast<size_t>(st.st_size);
    }
    else
    {
        perror("stat");
    }
}

StreamDataProvider::~StreamDataProvider()
{
    m_file.close();
}

size_t StreamDataProvider::size() const
{
    return m_file_size;
}

bool StreamDataProvider::mapChunk(size_t offset, size_t length, unsigned char *&buffer, size_t &actual_length)
{
    if (offset >= m_file_size)
    {
        return false;
    }
    size_t read_len = std::min(length, m_file_size - offset);

    m_buffer_pool.emplace_back(read_len);
    std::vector<unsigned char> &buf = m_buffer_pool.back();

    m_file.seekg(offset);
    m_file.read(reinterpret_cast<char *>(buf.data()), read_len);

    if (!m_file)
    {
        return false;
    }

    buffer = buf.data();
    actual_length = read_len;

    chunk_map[buffer] = {buffer, read_len, offset};
    return true;
}

bool StreamDataProvider::unmapChunk(unsigned char *buffer)
{
    auto it = chunk_map.find(buffer);
    if (it == chunk_map.end())
    {
        return false;
    }

    const ChunkInfo &info = it->second;

    m_file.seekp(info.offset_in_file);
    m_file.write(reinterpret_cast<const char *>(info.data), info.length);

    chunk_map.erase(it);

    for (auto pit = m_buffer_pool.begin(); pit != m_buffer_pool.end(); ++pit)
    {
        if (pit->data() == buffer)
        {
            m_buffer_pool.erase(pit);
            break;
        }
    }
    return true;
}
