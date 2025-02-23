#ifndef PADDR_H
#define PADDR_H

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include "common.h"

using index_t = std::vector<byte_t>::size_type;

// Singleton
class Memory
{
public:
    static Memory& get_instance()
    {
        static Memory instance;
        return instance;
    }

    Memory(Memory const&) = delete;
    Memory& operator=(Memory const&) = delete;

    void init_pmem(std::uint32_t size);

    inline index_t guest_to_host(const paddr_t addr) const { return addr - MBASE; }
    inline paddr_t host_to_guest(const index_t idx) const { return idx + MBASE; }
    inline auto load_addr() { return m_pmem.data(); }

    inline bool in_pmem(const paddr_t addr) const { return addr - MBASE < MSIZE; }

    template<typename T>
    T read(const index_t idx) const;

    /*
     * Physical Memory write functions
     * 
     * @param idx: index from 0
     * @param data: data to write
    */
    void write(const index_t idx, const byte_t data);
    void write(const index_t idx, const half_t data);
    void write(const index_t idx, const word_t data);
private:
    Memory() = default;

    std::vector<byte_t> m_pmem;
    bool m_initialized{};
};

/*
 * Physical Memory read functions
 * T: Type to read, e.g. byte_t, half_t, word_t
 * 
 * @param idx: index from 0
 * @return: data read from memory, type T
*/
template<typename T>
T Memory::read(const index_t idx) const
{
    T data{};

    if constexpr (std::is_same_v<T, byte_t>)
    {
        data = m_pmem[idx];
    }
    else if constexpr (std::is_same_v<T, half_t>)
    {
        data |= m_pmem[idx];
        data |= m_pmem[idx + 1] << 8;
    }
    else if constexpr (std::is_same_v<T, word_t>)
    {
        data |= m_pmem[idx];
        data |= m_pmem[idx + 1] << 8;
        data |= m_pmem[idx + 2] << 16;
        data |= m_pmem[idx + 3] << 24;
    }
    else
    {
        throw std::invalid_argument("Unsupported type for read");
    }

    return data;
}

/*
 * Memory API
 */

/*
 * Initialize physical memory (only called once)
*/
void init_mem(std::uint32_t size = MSIZE);

/*
 * Load image(bin only) into physical memory
 * 
 * @param filename: filename of the image to load
 * @return: number of bytes loaded
*/
std::uint32_t load_img(const std::string& filename);

/*
 * memory read (including MMIO)
 * 
 * @param addr: physical address to read
*/
template<typename T>
T pmem_read(const paddr_t addr)
{
    T data{};
    index_t idx{ Memory::get_instance().guest_to_host(addr)};

    if (Memory::get_instance().in_pmem(addr))
    {
        data = Memory::get_instance().read<T>(idx);
    }
    else
    {
        throw std::out_of_range("Unsupported memory access");
        // TODO: MMIO support
    }

    return data;
}

/*
 * memory write (including MMIO)
 * 
 * @param addr: physical address to write
 * @param data: data to write
*/
void pmem_write(const paddr_t addr, const auto data)
{
    index_t idx{ Memory::get_instance().guest_to_host(addr)};

    if (Memory::get_instance().in_pmem(addr))
    {
        Memory::get_instance().write(idx, data);
    }
    else
    {
        throw std::out_of_range("Unsupported memory access");
        // TODO: MMIO support
    }
}

#endif // PADDR_H