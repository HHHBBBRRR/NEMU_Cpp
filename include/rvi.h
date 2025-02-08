#ifndef RVI
#define RVI

#include "instruction.h"

// opcode: 0b0000011(3)
class LoadInst : public ITypeInst		
{
public:
	void execute(CPU& cpu) override;
};

// opcode: 0b0010011(19)
class ArithLogicImmInst : public ITypeInst
{
public:
	void execute(CPU& cpu) override;
};

// opcode: 0b0010111(23)
class AUIPCInst : public UTypeInst
{
public:
	void execute(CPU& cpu) override;
};

// opcode: 0b0100011(35)
class StoreInst : public STypeInst
{
public:
	void execute(CPU& cpu) override;
};

// opcode: 0b0110011(51)
class ArithLogicRegInst : public RTypeInst
{
public:
	void execute(CPU& cpu) override;
};

// opcode: 0b0110111(55)
class LUIInst : public UTypeInst
{
public:
	void execute(CPU& cpu) override;
};

// opcode: 0b1100011(99)
class BranchInst : public BTypeInst
{
public:
	void execute(CPU& cpu) override;
};

// opcode: 0b1100111(103)
class JALRInst : public ITypeInst
{
public:
	void execute(CPU& cpu) override;
};

// opcode: 0b1101111(111)
class JALInst : public JTypeInst
{
public:
	void execute(CPU& cpu) override;
};

#endif // RVI