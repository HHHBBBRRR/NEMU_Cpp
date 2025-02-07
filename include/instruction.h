#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <bitset>
#include "common.h"

using std::bitset;

class CPU
{
private:
    /* data */
public:
    CPU(/* args */);
    ~CPU();
};

class Instruction
{
public:
	virtual ~Instruction() = default;
	virtual void decode(word_t inst) = 0;
	virtual void execute(CPU& cpu) = 0;
};

class RTypeInst : public Instruction
{
public:
	virtual ~RTypeInst() = default;
	virtual void execute(CPU& cpu) = 0;

	void decode(word_t inst) override
	{
		m_rd = inst >> 7;
        m_funct3 = inst >> 12;
        m_rs1 = inst >> 15;
		m_rs2 = inst >> 20;
        m_funct7 = inst >> 25;
	}

	inline auto getRd() const { return m_rd; }
	inline auto getRs1() const { return m_rs1; }
	inline auto getRs2() const { return m_rs2; }
	inline auto getFunct3() const { return m_funct3; }
	inline auto getFunct7() const { return m_funct7; }
private:
	bitset<5> m_rd{};
    bitset<3> m_funct3{};
	bitset<5> m_rs1{}, m_rs2{};
    bitset<7> m_funct7{};
};

class ITypeInst : public Instruction
{
public:
	virtual ~ITypeInst() = default;
	virtual void execute(CPU& cpu) = 0;

	void decode(word_t inst) override
	{
		m_rd = inst >> 7;
        m_funct3 = inst >> 12;
        m_rs1 = inst >> 15;
		m_imm = inst >> 20;
		if (m_imm.test(11))
		{
			m_imm |= 0xFFFF'F000;
		}
	}

	inline auto getRd() const { return m_rd; }
	inline auto getRs1() const { return m_rs1; }
	inline auto getFunct3() const { return m_funct3; }
	inline auto getImm() const { return m_imm; }
private:
	bitset<5> m_rd{};
	bitset<3> m_funct3{};
	bitset<5> m_rs1{};
    bitset<32> m_imm{};
};

class STypeInst : public Instruction
{
public:
	virtual ~STypeInst() = default;
	virtual void execute(CPU& cpu) = 0;

	void decode(word_t inst) override
	{
		m_funct3 = inst >> 12;
		m_rs1 = inst >> 15;
		m_rs2 = inst >> 20;
		
		bitset<32> temp{ inst };

		for (int i{}; i <= 4; i++)
		{
			m_imm[i] = temp[i + 7];
		}
		for (int i{ 5 }; i <= 10; i++)
		{
			m_imm[i] = temp[i + 20];
		}
		if (temp.test(31))
		{
			m_imm |= 0xFFFF'F800;
		}
	}

	inline auto getRs1() const { return m_rs1; }
	inline auto getRs2() const { return m_rs2; }
	inline auto getImm() const { return m_imm; }
	inline auto getFunct3() const { return m_funct3; }
private:
	bitset<3> m_funct3{};
	bitset<5> m_rs1{}, m_rs2{};
    bitset<32> m_imm{};
};

class BTypeInst : public Instruction
{
public:
	virtual ~BTypeInst() = default;
	virtual void execute(CPU& cpu) = 0;

	void decode(word_t inst) override
	{
		m_funct3 = inst >> 12;
		m_rs1 = inst >> 15;
		m_rs2 = inst >> 20;
		
		bitset<32> temp{ inst };

		m_imm[0] = 0;
		for (int i{ 1 }; i <= 4; i++)
		{
			m_imm[i] = temp[i + 7];
		}
		for (int i{ 5 }; i <= 10; i++)
		{
			m_imm[i] = temp[i + 20];
		}
		m_imm[11] = temp[7];
		if (temp.test(31))
		{
			m_imm |= 0xFFFF'F000;
		}
	}

	inline auto getRs1() const { return m_rs1; }
	inline auto getRs2() const { return m_rs2; }
	inline auto getImm() const { return m_imm; }
    inline auto getFunct3() const { return m_funct3; }
private:
	bitset<3> m_funct3{};
    bitset<5> m_rs1{}, m_rs2{};
    bitset<32> m_imm{};
};

class UTypeInst : public Instruction
{
public:
	virtual ~UTypeInst() = default;
	virtual void execute(CPU& cpu) = 0;

    void decode(word_t inst) override
	{
		m_rd = inst >> 7;
		m_imm = inst & 0xFFFF'F000;
	}

	inline auto getRd() const { return m_rd; }
    inline auto getImm() const { return m_imm; }
private:
    bitset<5> m_rd{};
    bitset<32> m_imm{};
};

class JTypeInst : public Instruction
{
public:
    virtual ~JTypeInst() = default;
	virtual void execute(CPU& cpu) = 0;

	void decode(word_t inst) override
	{
		m_rd = inst >> 7;

		bitset<32> temp{ inst };

		m_imm[0] = 0;
		for (int i{ 1 }; i <= 10; i++)
		{
			m_imm[i] = temp[i + 20];
		}
		m_imm[11] = temp[20];
		for (int i{ 12 }; i <= 19; i++)
		{
			m_imm[i] = temp[i];
		}
		if (temp.test(31))
		{
			m_imm |= 0xFFF0'0000;
		}
	}

    inline auto getRd() const { return m_rd; }
    inline auto getImm() const { return m_imm; }
private:
    bitset<5> m_rd{};
    bitset<32> m_imm{};
};

#endif // INSTRUCTION_H