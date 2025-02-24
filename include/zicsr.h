#ifndef ZICSR_H
#define ZICSR_H

#include "instruction.h"

// opcode: 0b1110011(115)
class CSRInst : public ITypeInst
{
public:
    void execute(CPU& cpu) override;
};

#endif // !ZICSR_H