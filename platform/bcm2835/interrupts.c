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
#include <sys/types.h>
#include <debug.h>
#include <trace.h>
#include <reg.h>
#include <kernel/thread.h>
#include <kernel/debug.h>
#include <platform/interrupts.h>
#include <arch/ops.h>
#include <arch/arm.h>
#include <platform/bcm2835.h>
#include "platform_p.h"

struct int_handler_struct {
	int_handler handler;
	void *arg;
};

static struct int_handler_struct int_handler_table[BCM2835_NUM_IRQS];

void register_int_handler(unsigned int vector, int_handler handler, void *arg)
{
	if (vector >= BCM2835_NUM_IRQS)
		panic("%s: vector out of range %d\n", __PRETTY_FUNCTION__, vector);

	enter_critical_section();

	int_handler_table[vector].arg = arg;
	int_handler_table[vector].handler = handler;

	exit_critical_section();
}

void platform_init_interrupts(void)
{
	for (int n=0; n<BCM2835_NUM_IRQS; n++) {
		int_handler_table[n].arg = NULL;
		int_handler_table[n].handler = NULL;
	}

	// mask all interrupts
	writel(0x00000000, BCM2835_INTC_IRQ1DISABLE);
	writel(0x00000000, BCM2835_INTC_IRQ2DISABLE);
	writel(0x00000000, BCM2835_INTC_IRQBDISABLE);
	
	// clear any pending interrupts
	writel(0x00000000, BCM2835_INTC_IRQBPENDING);
	writel(0x00000000, BCM2835_INTC_IRQ1PENDING);
	writel(0x00000000, BCM2835_INTC_IRQ2PENDING);

	// unmask all interrupts
	writel(0x00000000, BCM2835_INTC_IRQ1ENABLE);
	writel(0x00000000, BCM2835_INTC_IRQ2ENABLE);
	writel(0x00000000, BCM2835_INTC_IRQBENABLE);
}

status_t mask_interrupt(unsigned int vector)
{
	if (vector >= BCM2835_NUM_IRQS)
		panic("%s: vector out of range %d\n", __PRETTY_FUNCTION__, vector);
//		return ERR_INVALID_ARGS;

	enter_critical_section();

    if (vector < 32) {
		writel(1 << vector, BCM2835_INTC_IRQ1DISABLE);
    }
    else if (vector < 64) {
		writel(1 << (vector - 32), BCM2835_INTC_IRQ2DISABLE);
    }
    else {
		writel(1 << (vector - 64), BCM2835_INTC_IRQBDISABLE);
    }

	exit_critical_section();

	return NO_ERROR;
}

status_t unmask_interrupt(unsigned int vector)
{
	if (vector >= BCM2835_NUM_IRQS)
		panic("%s: vector out of range %d\n", __PRETTY_FUNCTION__, vector);
//		return ERR_INVALID_ARGS;

	enter_critical_section();

    if (vector < 32) {
		writel(1 << vector, BCM2835_INTC_IRQ1ENABLE);
    }
    else if (vector < 64) {
		writel(1 << (vector - 32), BCM2835_INTC_IRQ2ENABLE);
    }
    else {
		writel(1 << (vector - 64), BCM2835_INTC_IRQBENABLE);
    }

	exit_critical_section();

	return NO_ERROR;
}

static bool check_irq_pending(uchar irq_num)
{
    /* Check the appropriate hardware register, depending on the IRQ number.  */
    if (irq_num >= 64) {
        if (readl(BCM2835_INTC_IRQBPENDING) & (1 << (irq_num - 64))) {
			return true;
        }
    }
    else if (irq_num >= 32) {
        if (readl(BCM2835_INTC_IRQ2PENDING) & (1 << (irq_num - 32))) {
			return true;
        }
    }
    else {
        if (readl(BCM2835_INTC_IRQ1PENDING) & (1 << irq_num)) {
			return true;
        }
    }

	return false;
}

enum handler_return platform_irq(struct arm_iframe *frame)
{
	THREAD_STATS_INC(interrupts);

	// get the current vector
	unsigned int vector;
	enum handler_return ret = INT_NO_RESCHEDULE;

	for (vector=0; vector<BCM2835_NUM_IRQS; vector++) {
		if (check_irq_pending(vector)) {
//	TRACEF("spsr 0x%x, pc 0x%x, currthread %p, vector %d, handler %p\n", frame->spsr, frame->pc, current_thread, vector, int_handler_table[vector].handler);
			// deliver the interrupt

			ret = INT_NO_RESCHEDULE;
			if (int_handler_table[vector].handler) {
				ret = int_handler_table[vector].handler(int_handler_table[vector].arg);
			}
			break;
		}
	}

	return ret;
}

void platform_fiq(struct arm_iframe *frame)
{
	TRACE;
	PANIC_UNIMPLEMENTED;
}

/* vim: set ts=4 sw=4 noexpandtab: */
