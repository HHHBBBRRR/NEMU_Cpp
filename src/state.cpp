#include "state.h"

NEMUState nemu_state;

void NEMUState::set_nemu_state(STATE state, paddr_t halt_pc, std::uint32_t halt_ret)
{
    m_state = state;
    m_halt_pc = halt_pc;
    m_halt_ret = halt_ret;
}

void nemu_trap(paddr_t thispc, std::uint32_t code)
{
    nemu_state.set_nemu_state(STATE::NEMU_END, thispc, code);
}

void invalid_inst(paddr_t thispc)
{
    std::cout << "Invalid instruction at pc = " << thispc << std::endl;
    nemu_state.set_nemu_state(STATE::NEMU_ABORT, thispc, 1);
}

void nemu_abort(paddr_t thispc)
{
    std::cout << "NEMU abort at pc = " << thispc << std::endl;
    nemu_state.set_nemu_state(STATE::NEMU_ABORT, thispc, 1);
}
