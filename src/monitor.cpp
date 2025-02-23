#include <iostream>
#include <getopt.h>
#include "paddr.h"
#include "monitor.h"

void Monitor::cpu_exec(std::uint64_t n)
{
	switch (m_nemu_state.m_state)
	{
	case STATE::NEMU_END:
	case STATE::NEMU_ABORT:
		std::cout << "Program execution has ended. To restart the program, exit NEMU and run again." << std::endl;
		return;
	default:
        m_nemu_state.m_state = STATE::NEMU_RUNNING;
		break;
	}

	execute(n);

	switch (m_nemu_state.m_state)
	{
	case STATE::NEMU_RUNNING:
		m_nemu_state.m_state = STATE::NEMU_STOP;
		break;
	case STATE::NEMU_END:
    case STATE::NEMU_ABORT:
		std::cout << "nemu: " <<
        (m_nemu_state.m_state == STATE::NEMU_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
        (m_nemu_state.m_halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
        ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))) <<
        "at pc = " << m_nemu_state.m_halt_pc << std::endl;
		break;
	}
}

void Monitor::set_nemu_state(STATE state, paddr_t halt_pc, std::uint32_t halt_ret)
{
    m_nemu_state.m_state = state;
    m_nemu_state.m_halt_pc = halt_pc;
    m_nemu_state.m_halt_ret = halt_ret;
}

void Monitor::invalid_inst(paddr_t thispc)
{
    std::cout << "Invalid instruction at pc = " << thispc << std::endl;
    set_nemu_state(STATE::NEMU_ABORT, thispc, 1);
}

Monitor::Monitor(std::string_view img)
{
	init_mem();
	load_img(img);
}

void Monitor::execute(std::uint64_t n)
{
	for (; n > 0; n--)
	{
		m_cpu.isa_exec_once();
		if (m_nemu_state.m_state != STATE::NEMU_RUNNING)
		{
			return;
		}
	}
}

bool Monitor::is_exit_status_bad()
{
	bool good{};

	good = (m_nemu_state.m_state == STATE::NEMU_END && m_nemu_state.m_halt_ret == 0);

	return !good;
}
