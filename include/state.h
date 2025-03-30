#ifndef STATE_H
#define STATE_H

#include <iostream>
#include "common.h"

enum class STATE
{
	NEMU_RUNNING,
	NEMU_STOP,
	NEMU_END,   // guest exit successfully
	NEMU_ABORT, // guest exit failure
};

struct NEMUState
{
    STATE m_state;
    paddr_t m_halt_pc;
    std::uint32_t m_halt_ret;

    NEMUState() 
        : m_state{STATE::NEMU_RUNNING}, m_halt_pc{0x8000'0000}, m_halt_ret{1}
    {}
    void set_nemu_state(STATE state, paddr_t halt_pc, std::uint32_t halt_ret);
};

extern NEMUState nemu_state;

void nemu_trap(paddr_t thispc, std::uint32_t code);

void invalid_inst(paddr_t thispc);

void nemu_abort(paddr_t thispc);

#endif // !STATE_H