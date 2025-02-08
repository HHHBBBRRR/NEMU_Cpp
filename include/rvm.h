#include "instruction.h"
#include "rvi.h"

// opcode: 0b0110011 (51)
class MulDivInst : public RTypeInst
{
public:
	void execute(CPU& cpu) override;
};

// Arithmetic Logic Multiplication Division Instruction
class ALMDInst : public RTypeInst
{
public:
	void execute(CPU& cpu) override;
private:
	ArithLogicRegInst m_arithLogicRegInst;
	MulDivInst m_mulDivInst;
};
