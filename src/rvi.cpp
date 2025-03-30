#include <bitset>
#include <cstdint>
#include <stdexcept>
#include "paddr.h"
#include "cpu.h"
#include "rvi.h"

void LoadInst::execute(CPU& cpu)
{
	auto rd{ getRd().to_ulong() }, rs1{ getRs1().to_ulong() };
	auto funct3{ getFunct3().to_ulong() };
	auto imm{ getImm().to_ulong() };
	word_t rs1Val{ cpu.getGPR(rs1) };
	paddr_t addr{ rs1Val + imm };
	std::bitset<32> rdVal{};

	switch (funct3)
	{
	case 0b000:		// lb    rd, imm(rs1)
		rdVal = pmem_read<byte_t>(addr);
		if (rdVal.test(7))
		{
			rdVal |= 0xFFFF'FF00;
		}
		break;
    case 0b001:		// lh    rd, imm(rs1)
        rdVal = pmem_read<half_t>(addr);
        if (rdVal.test(15))
        {
            rdVal |= 0xFFFF'0000;
        }
        break;
	case 0b010:		// lw    rd, imm(rs1)
        rdVal = pmem_read<word_t>(addr);
		break;
    case 0b100:		// lbu   rd, imm(rs1)
        rdVal = pmem_read<byte_t>(addr);
		break;
	case 0b101:		// lhu   rd, imm(rs1)
		rdVal = pmem_read<half_t>(addr);
		break;
	default:
		throw std::invalid_argument{ "Invalid funct3 value" };
		break;
	}

	cpu.setGPR(rd, rdVal.to_ulong());
}

void ArithLogicImmInst::execute(CPU& cpu)
{
	auto rd{ getRd().to_ulong() }, rs1{ getRs1().to_ulong() };
	auto funct3{ getFunct3().to_ulong() };
	auto imm{ getImm().to_ulong() };
	word_t rs1Val{ cpu.getGPR(rs1) };
	word_t rdVal{};

	switch (funct3)
	{
	case 0b000:	// addi rd, rs1, imm
		rdVal = rs1Val + imm;
		break;
	case 0b001:	// slli rd, rs1, imm
		rdVal = rs1Val << (imm & 0b11111);
		break;
	case 0b010:	// slti rd, rs1, imm
		rdVal = static_cast<std::int32_t>(rs1Val) < imm;
		break;
	case 0b011:	// sltui rd, rs1, imm				
		rdVal = rs1Val < static_cast<std::uint32_t>(imm);
		break;
	case 0b100:	// xori  rd, rs1, imm
		rdVal = rs1Val ^ imm;
		break;
	case 0b101:
		if (imm & 0b0100'0000'0000) // srli rd, rs1, imm
		{
			rdVal = static_cast<std::int32_t>(rs1Val) >> (imm & 0b11111);
		}
		else	      // srai  rd, rs1, rs2
		{
			rdVal = rs1Val >> (imm & 0b11111);
		}
		break;
	case 0b110:	// ori   rd, rs1, imm
		rdVal = rs1Val | imm;
		break;
	case 0b111:	// andi  rd, rs1, imm
		rdVal = rs1Val & imm;
		break;
	default:
		throw std::invalid_argument{ "Invalid funct3 value" };
		break;
	}

	cpu.setGPR(rd, rdVal);
}

void AUIPCInst::execute(CPU& cpu)		// auipc rd, upimm
{
	auto rd{ getRd().to_ulong() };
	auto imm{ getImm().to_ulong() };
	word_t rdVal{};

	rdVal = cpu.getPC() + imm;

	cpu.setGPR(rd, rdVal);
}

void StoreInst::execute(CPU& cpu)
{
	auto rs1{ getRs1() }, rs2{ getRs2() };
	auto funct3{ getFunct3().to_ulong() };
	auto imm{ getImm().to_ulong() };
	word_t rs1Val{ cpu.getGPR(rs1.to_ulong()) };
	word_t rs2Val{ cpu.getGPR(rs2.to_ulong()) };
	paddr_t addr{ rs1Val + imm };

    switch (funct3)
    {
    case 0b000:		// sb    rs2, imm(rs1)
		pmem_write(addr, static_cast<byte_t>(rs2Val));
        break;
    case 0b001:		// sh    rs2, imm(rs1)
        pmem_write(addr, static_cast<half_t>(rs2Val));
        break;
    case 0b010:		// sw    rs2, imm(rs1)
		pmem_write(addr, static_cast<word_t>(rs2Val));
        break;
	default:
		throw std::invalid_argument{ "Invalid funct3 value" };
		break;
    }
}

void ArithLogicRegInst::execute(CPU& cpu)
{
	auto rd{ getRd() }, rs1{ getRs1() }, rs2{ getRs2() };
	auto funct3{ getFunct3().to_ulong() };
	auto funct7{ getFunct7() };
	auto func7Bit5{ funct7.test(5)};
	word_t rs1Val{ cpu.getGPR(rs1.to_ulong()) };
	word_t rs2Val{ cpu.getGPR(rs2.to_ulong()) };
	word_t rdVal{};

	switch (funct3)
	{
	case 0b000:
		if (!func7Bit5)
		{
			rdVal = rs1Val + rs2Val;	// add  rd, rs1, rs2
		}
		else
		{
			rdVal = rs1Val - rs2Val;	// sub  rd, rs1, rs2
		}
		break;
	case 0b001:							// sll  rd, rs1, rs2
		rdVal = rs1Val << (rs2Val & 0b11111);
		break;
	case 0b010:							// slt  rd, rs1, rs2
		rdVal = static_cast<std::int32_t>(rs1Val) < static_cast<std::int32_t>(rs2Val);
		break;
	case 0b011:							// sltu rd, rs1, rs2
		rdVal = rs1Val < rs2Val;
		break;
	case 0b100:							// xor  rd, rs1, rs2
		rdVal = rs1Val ^ rs2Val;
		break;
	case 0b101:
		if (!func7Bit5)					// srl  rd, rs1, rs2
		{
			rdVal = rs1Val >> (rs2Val & 0b11111);
		}
		else							// sra  rd, rs1, rs2
		{
			rdVal = static_cast<std::int32_t>(rs1Val) >> (rs2Val & 0b11111);
		}
		break;
	case 0b110:							// or   rd, rs1, rs2
		rdVal = rs1Val | rs2Val;
		break;
	case 0b111:							// and  rd, rs1, rs2
		rdVal = rs1Val & rs2Val;
		break;
	default:
		throw std::invalid_argument{ "Invalid funct3 value" };
		break;
	}

	cpu.setGPR(rd.to_ulong(), rdVal);
}

void BranchInst::execute(CPU& cpu)
{
	auto rs1{ getRs1() }, rs2{ getRs2() };
	auto imm{ getImm().to_ulong()};
	auto funct3{ getFunct3().to_ulong()};
	word_t rs1Val{ cpu.getGPR(rs1.to_ulong()) };
	word_t rs2Val{ cpu.getGPR(rs2.to_ulong()) };
	word_t targetAddr{};
	bool branch{};

	switch (funct3)
	{
	case 0b000:	// beq  rs1, rs2, label
		if (rs1Val == rs2Val)
		{
			branch = true;
		}
		break;
	case 0b001: // bne  rs1, rs2, label
		if (rs1Val != rs2Val)
		{
			branch = true;
		}
		break;
	case 0b100: // blt  rs1, rs2, label
		if (static_cast<std::int32_t>(rs1Val) < static_cast<std::int32_t>(rs2Val))
		{
			branch = true;
		}
		break;
	case 0b101: // bge  rs1, rs2, label
		if (static_cast<std::int32_t>(rs1Val) >= static_cast<std::int32_t>(rs2Val))
		{
			branch = true;
		}
		break;
	case 0b110:	// bltu rs1, rs2, label
		if (rs1Val < rs2Val)
		{
			branch = true;
		}
		break;
	case 0b111: // bgeu rs1, rs2, label
		if (rs1Val >= rs2Val)
		{
			branch = true;
		}
		break;
	default:
		throw std::invalid_argument{ "Invalid funct3 value" };
		break;
	}

	if (branch)
	{
		targetAddr = cpu.getPC() + static_cast<std::int32_t>(imm);
		/* The conditional branch instructions will generate an instruction-address-misaligned exception if the
   target address is not aligned to a four-byte boundary and the branch condition evaluates to true. If the
   branch condition evaluates to false, the instruction-address-misaligned exception will not be raised.*/
		if (targetAddr % 4)
		{
			throw std::invalid_argument{ "Target address is not aligned" };
		}
		cpu.setPC(targetAddr);
	}
}

void LUIInst::execute(CPU& cpu)		// lui rd, upimm
{
	auto rd{ getRd().to_ulong()};
	auto imm{ getImm().to_ulong()};
	word_t rdVal{ static_cast<std::uint32_t>(imm) };

	cpu.setGPR(rd, rdVal);
}

void JALRInst::execute(CPU& cpu)	// jalr rd, rs1, imm
{
	auto rd{ getRd().to_ulong()};
	auto imm{ getImm().to_ulong()};
	word_t rs1Val{ cpu.getGPR(getRs1().to_ulong()) };
	word_t targetAddr{};

	targetAddr = rs1Val + static_cast<std::int32_t>(imm);
	if (targetAddr % 4)
	{
		throw std::invalid_argument{ "Target address is not aligned" };
	}

	cpu.setGPR(rd, cpu.getPC() + 4);
	cpu.setPC(targetAddr);
}

void JALInst::execute(CPU& cpu)		// jal rd, label
{
	auto rd{ getRd().to_ulong() };
	auto imm{ getImm().to_ulong() };
	word_t rdVal{};
	word_t targetAddr{};

	rdVal = cpu.getPC() + 4;
	targetAddr = cpu.getPC() + static_cast<std::int32_t>(imm);
	if (targetAddr % 4)
	{
		throw std::invalid_argument{ "Target address is not aligned" };
	}

	cpu.setPC(targetAddr);
	cpu.setGPR(rd, rdVal);
}