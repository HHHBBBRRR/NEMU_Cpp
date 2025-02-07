#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

using byte_t = std::uint8_t;
using half_t = std::uint16_t;
using word_t = std::uint32_t;
using paddr_t = std::uint64_t;	// The RV32 system supports 34 bits of physical address, so we use uint64_t
using vaddr_t = std::uint32_t;

constexpr std::uint64_t MBASE{ 0x8000'0000 };
constexpr std::uint64_t MSIZE{ 1024 * 1024 }; // 1MB

constexpr std::uint32_t NUM_REGS{ 32 };

constexpr std::uint32_t XLEN{ 32 };

#define RVM

#endif // COMMON_H