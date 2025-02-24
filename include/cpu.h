#ifndef CPU_H
#define CPU_H

#include <array>
#include <bitset>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include "common.h"

class Instruction;

class InstructionFactory
{
public:
	InstructionFactory();
	std::shared_ptr<Instruction> createInstruction(word_t inst);

private:
	std::unordered_map<std::bitset<7>, std::shared_ptr<Instruction>> m_instructionMap;
};

class CPU
{
public:
	word_t getGPR(std::uint32_t idx) const;
	void setGPR(std::uint32_t idx, word_t value);
	word_t getPC() const;
	void setPC(word_t value);
	void isa_exec_once();
	void isa_reg_display() const;
private:
	inline word_t inst_fetch() const;

	std::array<word_t, NUM_REGS> m_GPR{};		// General Purpose Register
	word_t m_PC{ MBASE };		// Program Counter

	InstructionFactory m_factory;
};

#endif // CPU_H