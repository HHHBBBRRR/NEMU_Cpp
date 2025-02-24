#include <cstdint>
#include <string_view>
#include "common.h"
#include "cpu.h"

class Monitor
{
public:
    Monitor(std::string_view img);  
	void cpu_exec(std::uint64_t n);

private:
	void execute(std::uint64_t n);

	CPU m_cpu;
};