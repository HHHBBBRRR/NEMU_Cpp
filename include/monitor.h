#include <cstdint>
#include <string_view>
#include "common.h"
#include "cpu.h"
#include <cstdlib>

enum class STATE
{
	NEMU_RUNNING,
	NEMU_STOP,
	NEMU_END,   // guest exit successfully
	NEMU_ABORT, // guest exit failure
};

class Monitor
{
public:
	struct NEMUState
	{
		STATE m_state;
		paddr_t m_halt_pc;
        std::uint32_t m_halt_ret;

        NEMUState() 
            : m_state{STATE::NEMU_RUNNING}, m_halt_pc{0x8000'0000}, m_halt_ret{1}
        {}
	};

	void cpu_exec(std::uint64_t n);
    void inline nemu_trap(paddr_t thispc, std::uint32_t code)
    {
        set_nemu_state(STATE::NEMU_END, thispc, code);
    }
    void invalid_inst(paddr_t thispc);

	Monitor(std::string_view img);

private:
	void execute(std::uint64_t n);
	bool is_exit_status_bad();
    void set_nemu_state(STATE state, paddr_t halt_pc, std::uint32_t halt_ret);

	CPU m_cpu;
	NEMUState m_nemu_state;
};