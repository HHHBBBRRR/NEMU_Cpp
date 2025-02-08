#include <cstdint>
#include <stdexcept>
#include "rvm.h"

using SignDoubleWord = std::int64_t;
using UsignDoubleWord = std::uint64_t;

void MulDivInst::execute(CPU& cpu)
{
	auto rd{ getRd() }, rs1{ getRs1() }, rs2{ getRs2() };
	auto funct3{ getFunct3() };
	auto funct7{ getFunct7() };
	word_t rs1Val{ cpu.getGPR(rs1.to_ulong()) };
	word_t rs2Val{ cpu.getGPR(rs2.to_ulong()) };
	word_t rdVal{};

	if (funct7 != 0b0000001)
	{
		throw std::invalid_argument{ "Invalid funct7 value" };
	}

	switch (funct3.to_ulong())
	{
	case 0b000:	// mul    rd, rs1, rs2
		rdVal = rs1Val * rs2Val;
		break;
	case 0b001:	// mulh   rd, rs1, rs2							
		rdVal = (static_cast<SignDoubleWord>(rs1Val) * static_cast<SignDoubleWord>(rs2Val)) >> 32;
		break;
	case 0b010:	// mulhsu rd, rs1, rs2
		rdVal = (static_cast<SignDoubleWord>(rs1Val) * static_cast<UsignDoubleWord>(rs2Val)) >> 32;
		break;
	case 0b011:	// mulhu  rd, rs1, rs2						
		rdVal = (static_cast<UsignDoubleWord>(rs1Val) * static_cast<UsignDoubleWord>(rs2Val)) >> 32;
		break;
	case 0b100:	// div    rd, rs1, rs2					
		rdVal = static_cast<std::int32_t>(rs1Val) / static_cast<std::int32_t>(rs2Val);
		break;
	case 0b101:	// divu   rd, rs1, rs2
		rdVal = static_cast<std::uint32_t>(rs1Val) / static_cast<std::uint32_t>(rs2Val);
		break;
	case 0b110:	// rem	  rd, rs1, rs2						
		rdVal = static_cast<std::int32_t>(rs1Val) % static_cast<std::int32_t>(rs2Val);
		break;
	case 0b111:	// remu	  rd, rs1, rs2					
		rdVal = static_cast<std::uint32_t>(rs1Val) % static_cast<std::uint32_t>(rs2Val);
		break;
	default:
		throw std::invalid_argument{ "Invalid funct3 value" };
		break;
	}

	cpu.setGPR(rd.to_ulong(), rdVal);
}

void ALMDInst::execute(CPU& cpu)
{
	auto funct7{ getFunct7() };

	if (funct7 == 0b0000001)
	{
		m_mulDivInst.execute(cpu);
	}
	else
	{
		m_arithLogicRegInst.execute(cpu);
	}
}