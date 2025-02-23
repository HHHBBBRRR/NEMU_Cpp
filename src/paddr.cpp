#include <string_view>
#include <fstream>
#include <cstdint>
#include <stdexcept>
#include "paddr.h"

void Memory::init_pmem(std::uint32_t size)
{
    if (m_initialized)
    {
        throw std::runtime_error("Memory has been initialized");
    }

    m_pmem.resize(size);
    m_initialized = true;
}

void Memory::write(const index_t idx, const byte_t data)
{
    m_pmem[idx] = data;
}

void Memory::write(const index_t idx, const half_t data)
{
    m_pmem[idx] = static_cast<byte_t>(data);
    m_pmem[idx + 1] = static_cast<byte_t>(data >> 8);
}

void Memory::write(const index_t idx, const word_t data)
{
    m_pmem[idx] = static_cast<byte_t>(data);
    m_pmem[idx + 1] = static_cast<byte_t>(data >> 8);
    m_pmem[idx + 2] = static_cast<byte_t>(data >> 16);
    m_pmem[idx + 3] = static_cast<byte_t>(data >> 24);
}

void init_mem(std::uint32_t size)
{
    Memory::get_instance().init_pmem(size);
}

std::uint32_t load_img(std::string_view filename)
{
    std::ifstream img(filename.data(), std::ios::binary);

    if (!img)
    {
        throw std::runtime_error("Failed to open image file");
    }

    img.seekg(0, std::ios::end);
    auto size = img.tellg();
    img.seekg(0, std::ios::beg);

    if (size > MSIZE)
    {
        throw std::runtime_error("Image file is too large");
    }

    if (!img.read(reinterpret_cast<char*>(Memory::get_instance().load_addr()), size))
    {
        throw std::runtime_error("Failed to read image file");
    }

    img.close();

    return size;
}