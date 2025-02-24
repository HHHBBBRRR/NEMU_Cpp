#include <iostream>
#include "paddr.h"
#include "monitor.h"
#include "state.h"

Monitor::Monitor(std::string_view img)
{
	init_mem();
	load_img(img);
}

void Monitor::cpu_exec(std::uint64_t n)
{
	switch (nemu_state.m_state)
	{
	case STATE::NEMU_END:
	case STATE::NEMU_ABORT:
		std::cout << "Program execution has ended. To restart the program, exit NEMU and run again." << std::endl;
		return;
	default:
		nemu_state.m_state = STATE::NEMU_RUNNING;
		break;
	}

	execute(n);

	switch (nemu_state.m_state)
	{
	case STATE::NEMU_RUNNING:
		nemu_state.m_state = STATE::NEMU_STOP;
		break;
	case STATE::NEMU_END:
    case STATE::NEMU_ABORT:
		std::cout << "nemu: " <<
        (nemu_state.m_state == STATE::NEMU_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
        (nemu_state.m_halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
        ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))) <<
        "at pc = " << nemu_state.m_halt_pc << std::endl;
		break;
	}
}

void Monitor::execute(std::uint64_t n)
{
	for (; n > 0; n--)
	{
		m_cpu.isa_exec_once();
		if (nemu_state.m_state != STATE::NEMU_RUNNING)
		{
			return;
		}
	}
}
