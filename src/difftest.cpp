/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

extern "C" {
#include <dlfcn.h>
}

#include <cassert>
#include <string_view>
#include <iostream>
#include "difftest.h"
#include "state.h"

#ifdef CONFIG_DIFFTEST

static diff_context_t context;

void (*ref_difftest_init)(std::string_view img, std::uint64_t img_size);
void (*ref_difftest_regcpy)(diff_context_t *context, DIRECTION direction);
void (*ref_difftest_exec)(uint64_t n);

static void checkregs(CPU& dut, diff_context_t& context)
{
	bool is_ok{ true };

	if (dut.getPC() != context.pc)
	{
		is_ok = false;
	}

	for (int i{}; i < NUM_REGS; i++)
	{
		if (dut.getGPR(i) != context.gpr[i])
		{
			is_ok = false;
			break;
		}
	}
	
	if (!is_ok)
	{
		std::cout << ANSI_FMT("Difftest failed at pc = " << dut.getPC(), ANSI_FG_RED) << std::endl;
		dut.isa_reg_display();
		nemu_abort(dut.getPC());
	}
}

void init_difftest(char *ref_so_file, std::string_view img, std::uint64_t img_size)
{
	assert(ref_so_file != NULL);

	void *handle;
	handle = dlopen(ref_so_file, RTLD_LAZY);
	assert(handle);

	ref_difftest_init = dlsym(handle, "difftest_init");
	assert(ref_difftest_init);

	ref_difftest_regcpy = dlsym(handle, "difftest_regcpy");
	assert(ref_difftest_regcpy);

	ref_difftest_exec = dlsym(handle, "difftest_exec");
	assert(ref_difftest_exec);

	ref_difftest_init(img, img_size);
	ref_difftest_regcpy(&context, DIRECTION::DIFFTEST_TO_REF);
}

void difftest_step(CPU& dut)
{
	ref_difftest_exec(1);
	ref_difftest_regcpy(&context, DIRECTION::DIFFTEST_TO_DUT);

	checkregs(dut, context);
}

#endif
