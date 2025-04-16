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

#include "cpu.h"
#include "paddr.h"

CPU cpu;

void diff_get_regs(void *diff_context)
{
	struct diff_context_t *ctx = (struct diff_context_t *)diff_context;
	for (uint32_t i = 0; i < NUM_REGS; i++)
	{
		ctx->gpr[i] = cpu.getGPR(i);
	}
	ctx->pc = cpu.getPC();
}

void diff_set_regs(void *diff_context)
{
	struct diff_context_t *ctx = (struct diff_context_t *)diff_context;
	for (uint32_t i = 0; i < NUM_REGS; i++)
	{
		cpu.setGPR(i, ctx->gpr[i]);
	}
	cpu.setPC(ctx->pc);
}

extern "C" __EXPORT void difftest_init(std::string_view img, std::uint64_t img_size)
{
	init_mem();
	load_img(img);
}

extern "C" __EXPORT void difftest_regcpy(diff_context_t *context, DIRECTION direction)
{
	if (direction == DIRECTION::DIFFTEST_TO_REF)
	{
		diff_set_regs(context);
	}
	else
	{
		diff_get_regs(context);
	}
}

extern "C" __EXPORT void difftest_exec(uint64_t n)
{
	while (n > 0)
	{
		cpu.isa_exec_once();
		n--;
	}
}
