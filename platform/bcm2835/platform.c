/*
 * Copyright (c) 2012 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <err.h>
#include <debug.h>
#include <platform.h>
#include <dev/uart.h>
#include <arch/arm/mmu.h>
#include <platform/bcm2835.h>
#include "platform_p.h"

#define MEGABYTE (1024 * 1024)

void platform_init_mmu_mappings(void)
{
	/* do some memory map initialization */
	addr_t addr;

	// 1024MB - 16MB of kernel memory (some belongs to the VC)
	for (addr = 0x00; addr < MEMSIZE - (16 * MEGABYTE); addr += MEGABYTE) {
		arm_mmu_map_section(addr, addr, MMU_FLAG_CACHED | MMU_FLAG_BUFFERED | MMU_FLAG_READWRITE);
	}

#if BCM2835
	// unused up to PERIPHERALS_BASE
	for (; addr < PERIPHERALS_BASE; addr += MEGABYTE) {
		arm_mmu_map_section(addr, addr, 0);
	}
#else
	addr = PERIPHERALS_BASE;
#endif

	// 16 MB peripherals at PERIPHERALS_BASE
	for (; addr < PERIPHERALS_BASE + (16 * MEGABYTE); addr += MEGABYTE) {
		arm_mmu_map_section(addr, addr, MMU_FLAG_READWRITE); // 0x10416
	}

	// 1 MB mailboxes
	// shared device, never execute
	arm_mmu_map_section(addr, addr, MMU_FLAG_READWRITE);
	addr += MEGABYTE;

	// unused up to 0x7FFFFFFF
	for (; addr < 0x7FFFFFFF; addr += MEGABYTE) {
		arm_mmu_map_section(addr, addr, 0);
	}

	// one second level page tabel (leaf table) at 0x80000000
	arm_mmu_map_section(addr, addr, MMU_FLAG_CACHED);
	addr += MEGABYTE;

	// 2047MB unused (rest of address space)
}

void platform_early_init(void)
{
	uart_init_early();

	/* initialize the interrupt controller */
	platform_init_interrupts();

	/* initialize the timer block */
	platform_init_timer();
}

void platform_init(void)
{
}

