/*
 * Copyright (c) 2008-2012 Travis Geiselbrecht
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
#include <debug.h>
#include <sys/types.h>
#include <err.h>
#include <stdio.h>
#include <trace.h>
#include <kernel/thread.h>
#include <platform.h>
#include <platform/interrupts.h>
#include <platform/timer.h>
#include <platform/bcm2835.h>
#include "platform_p.h"

static platform_timer_callback t_callback;

static volatile uint ticks = 0;
static lk_time_t periodic_interval;

status_t platform_set_periodic_timer(platform_timer_callback callback, void *arg, lk_time_t interval)
{
	enter_critical_section();

	TRACEF("callback %p, arg %p, interval %lu\n", callback, arg, interval);

	t_callback = callback;

	periodic_interval = interval;

    volatile uint32_t rx = readl(SYSTIMER_CLO);
    rx += periodic_interval * 1000;
	writel(rx, SYSTIMER_C1);
	writel(2,  SYSTEM_TIMER_REGS_BASE);

	unmask_interrupt(IRQ_SYSTEM_TIMER_1);

	exit_critical_section();

	return NO_ERROR;
}

lk_bigtime_t current_time_hires(void)
{
	lk_bigtime_t time;

	time = ticks * periodic_interval * 1000ULL;

	return time;
}

lk_time_t current_time(void)
{
	lk_time_t time;

	time = ticks * periodic_interval;

	return time;
}

static enum handler_return platform_tick(void *arg)
{
	ticks++;

    //advance the timer comparison register to the next time we want an interrupt
    volatile uint32_t rx = readl(SYSTIMER_CLO);
    rx += periodic_interval * 1000;
	writel(rx, SYSTIMER_C1);

    //clear/ack the interrupt
	writel(2,  SYSTEM_TIMER_REGS_BASE);

	if (t_callback) {
		return t_callback(arg, current_time());
	} else {
		return INT_NO_RESCHEDULE;
	}
}

void platform_init_timer(void)
{
	register_int_handler(IRQ_SYSTEM_TIMER_1, &platform_tick, NULL);
}

/* vim: set ts=4 sw=4 noexpandtab: */
