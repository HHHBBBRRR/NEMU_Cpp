#include <stdexcept>
#include <cstdint>
#include "cpu.h"
#include "zicsr.h"
#include "state.h"

void CSRInst::execute(CPU& cpu)
{
    auto rd{ getRd().to_ulong() }, rs1{ getRs1().to_ulong() };
	auto funct3{ getFunct3().to_ulong() };
	auto imm{ getImm().to_ulong() };
	word_t rs1Val{ cpu.getGPR(rs1) };
	word_t rdVal{};

    switch (funct3)
    {
        case 0b000:
            if (rs1Val == 0 && rdVal == 0)
            {
                if (imm == 1)   // ebreak
                {
                    nemu_trap(cpu.getPC(), cpu.getGPR(10));
                }   
            }
            else
            {
                throw std::runtime_error("Unsupported CSR instruction");
            }   
            break;
        default:
            throw std::runtime_error("Unsupported CSR instruction");
    }
}