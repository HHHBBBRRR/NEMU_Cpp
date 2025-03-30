#include <iostream>
#include "paddr.h"
#include "monitor.h"
#include "state.h"
#include "difftest.h"

Monitor::Monitor(std::string_view img, std::string_view ref_so_file)
{
	init_mem();
	std::uint64_t img_size = load_img(img);
	init_difftest(ref_so_file.data(), img, img_size);
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
		if (nemu_state.m_state == STATE::NEMU_ABORT)
		{
			std::cout << std::hex << ANSI_FMT("NEMU_Cpp abort at PC = 0x" << nemu_state.m_halt_pc, ANSI_FG_RED) << std::endl;
		}
		else
		{
			if (nemu_state.m_halt_ret == 0)
			{
				std::cout << std::hex << ANSI_FMT("NEMU_Cpp hit good trap at PC = 0x" << nemu_state.m_halt_pc, ANSI_FG_GREEN) << std::endl;
			}
			else
			{
				std::cout << std::hex << ANSI_FMT("NEMU_Cpp hit bad trap at PC = 0x" << nemu_state.m_halt_pc, ANSI_FG_RED) << std::endl;
			}			
		}
		break;
	}
}

void Monitor::execute(std::uint64_t n)
{
	for (; n > 0; n--)
	{
		m_cpu.isa_exec_once();
		if (nemu_state.m_state == STATE::NEMU_END) // "ebreak" instruction means the program has ended
		{
			return;
		}
		difftest_step(m_cpu);
		if (nemu_state.m_state != STATE::NEMU_RUNNING)
		{
			return;
		}
	}
}
