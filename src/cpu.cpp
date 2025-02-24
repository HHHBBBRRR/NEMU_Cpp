#include <array>
#include <bitset>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <iostream>
#include "common.h"
#include "paddr.h"
#include "instruction.h"
#include "rvi.h"
#include "zicsr.h"
#include "state.h"
#include "cpu.h"

using std::make_shared;

const std::array<std::string, NUM_REGS> regs{
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

InstructionFactory::InstructionFactory()
{
    /* RVI */
    m_instructionMap[0b0000011] = make_shared<LoadInst>();
    m_instructionMap[0b0010011] = make_shared<ArithLogicImmInst>();
    m_instructionMap[0b0010111] = make_shared<AUIPCInst>();
    m_instructionMap[0b0100011] = make_shared<StoreInst>();
    m_instructionMap[0b0110011] = make_shared<ArithLogicRegInst>();
    m_instructionMap[0b0110111] = make_shared<LUIInst>();
    m_instructionMap[0b1100011] = make_shared<BranchInst>();
    m_instructionMap[0b1100111] = make_shared<JALRInst>();
    m_instructionMap[0b1101111] = make_shared<JALInst>();
    /* Zicsr */
    m_instructionMap[0b1110011] = make_shared<CSRInst>();
}

std::shared_ptr<Instruction> InstructionFactory::createInstruction(word_t inst)
{
    std::bitset<7> opcode{ inst };

    if (m_instructionMap.count(opcode))
    {
        return m_instructionMap[opcode];
    }
    else
    {
        throw std::runtime_error("Invalid instruction");
    }

    return nullptr;
}

word_t CPU::getGPR(std::uint32_t idx) const
{
	if (idx == 0)	//Register x0 is hardwired with all bits equal to 0
    {
        return 0;
    }
	return m_GPR.at(idx);
}

void CPU::setGPR(std::uint32_t idx, word_t value)
{
    if (idx == 0)	//Register x0 is hardwired with all bits equal to 0
    {
        return;
    }
	m_GPR.at(idx) = value;
}

word_t CPU::getPC() const
{
	return m_PC;
}

void CPU::setPC(word_t value)
{
	m_PC = value;
}

void CPU::isa_exec_once()
{
    word_t pc{ getPC() };
    word_t inst{ inst_fetch() };
    auto instruction{ m_factory.createInstruction(inst) };

    try
    {
        auto instruction{ m_factory.createInstruction(inst) };
    }
    catch(const std::exception& e)
    {
        invalid_inst(pc);
    }

    instruction->decode(inst);
    instruction->execute(*this);

    if (pc == getPC())  // not branch/jump instruction
    {
        setPC(pc + 4);
    }
}

void CPU::isa_reg_display() const
{
    std::cout << ANSI_FG_WHITE << ANSI_BG_BLUE << "PC: " << std::hex << getPC() << ANSI_NONE << std::endl;
    for (std::uint32_t i{}; i < NUM_REGS; ++i)
    {
        std::cout << std::left << std::setw(3) << regs[i] << ": " << std::setw(10) << std::hex << getGPR(i) << "\t";
        if ((i + 1) % 4 == 0)
        {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

inline word_t CPU::inst_fetch() const
{
    return pmem_read<word_t>(getPC());
}
