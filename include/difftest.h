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

#ifndef DIFFTEST_H
#define DIFFTEST_H

#include <string_view>
#include "common.h"
#include "cpu.h"

#ifdef CONFIG_DIFFTEST
void init_difftest(char *ref_so_file, std::string_view img, std::uint64_t img_size);
void difftest_step(CPU& dut);
#else
static inline void init_difftest(char *ref_so_file, std::string_view img, std::uint64_t img_size) {}
static inline void difftest_step(CPU& dut) {}
#endif

#endif // !DIFFTEST_H
