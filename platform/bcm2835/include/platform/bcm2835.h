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
#ifndef __BCM2835_H
#define __BCM2835_H

/********************************************************************
 * ARM physical memory addresses of selected BCM2835 peripherals    *
 ********************************************************************/

/* Start of memory-mapped peripherals address space  */
#if BCM2836
# define PERIPHERALS_BASE 0x3F000000
#elif BCM2835
# define PERIPHERALS_BASE 0x20000000
#else
# error "unknown Broadcom SOC"
#endif

/* System timer  */
#define SYSTEM_TIMER_REGS_BASE	(PERIPHERALS_BASE + 0x3000)
#define SYSTIMER_CLO			(SYSTEM_TIMER_REGS_BASE + 0x04)
#define SYSTIMER_C0				(SYSTEM_TIMER_REGS_BASE + 0x0c)
#define SYSTIMER_C1				(SYSTEM_TIMER_REGS_BASE + 0x10)
#define SYSTIMER_C2				(SYSTEM_TIMER_REGS_BASE + 0x14)
#define SYSTIMER_C3				(SYSTEM_TIMER_REGS_BASE + 0x18)

/* Interrupt controller (for ARM)  */
#define INTERRUPT_REGS_BASE    (PERIPHERALS_BASE + 0xB200)

/* Mailbox  */
#define MAILBOX_REGS_BASE      (PERIPHERALS_BASE + 0xB880)

/* Power management / watchdog timer  */
#define PM_REGS_BASE           (PERIPHERALS_BASE + 0x100000)

/* GPIO  */
#define GPIO_REGS_BASE         (PERIPHERALS_BASE + 0x200000)

/* PL011 UART  */
#define PL011_REGS_BASE        (GPIO_REGS_BASE + 0x15000)

/* SD host controller  */
#define SDHCI_REGS_BASE        (PERIPHERALS_BASE + 0x300000)

/* Synopsys DesignWare Hi-Speed USB 2.0 On-The-Go Controller  */
#define DWC_REGS_BASE          (PERIPHERALS_BASE + 0x980000)

/* Interrupt controller */
#define	BCM2835_INTC_IRQBPENDING	(INTERRUPT_REGS_BASE + 0x00)	/* IRQ Basic pending */
#define	BCM2835_INTC_IRQ1PENDING	(INTERRUPT_REGS_BASE + 0x04)	/* IRQ pending 1 */
#define	BCM2835_INTC_IRQ2PENDING	(INTERRUPT_REGS_BASE + 0x08)	/* IRQ pending 2 */
#define	BCM2835_INTC_FIQCTL			(INTERRUPT_REGS_BASE + 0x0c)	/* FIQ control */
#define	BCM2835_INTC_IRQ1ENABLE		(INTERRUPT_REGS_BASE + 0x10)	/* Enable IRQs 1 */
#define	BCM2835_INTC_IRQ2ENABLE		(INTERRUPT_REGS_BASE + 0x14)	/* Enable IRQs 2 */
#define	BCM2835_INTC_IRQBENABLE		(INTERRUPT_REGS_BASE + 0x18)	/* Enable Basic IRQs */
#define	BCM2835_INTC_IRQ1DISABLE	(INTERRUPT_REGS_BASE + 0x1c)	/* Disable IRQ 1 */
#define	BCM2835_INTC_IRQ2DISABLE	(INTERRUPT_REGS_BASE + 0x20)	/* Disable IRQ 2 */
#define	BCM2835_INTC_IRQBDISABLE	(INTERRUPT_REGS_BASE + 0x24)	/* Disable Basic IRQs */

#define	BCM2835_INTC_ENABLEBASE		BCM2835_INTC_IRQ1ENABLE
#define	BCM2835_INTC_DISABLEBASE	BCM2835_INTC_IRQ1DISABLE


/* Number of IRQs shared between the GPU and ARM. These correspond to the IRQs
 * that show up in the IRQ_pending_1 and IRQ_pending_2 registers.  */
#define BCM2835_NUM_GPU_SHARED_IRQS     64

/* Number of ARM-specific IRQs. These correspond to IRQs that show up in the
 * first 8 bits of IRQ_basic_pending.  */
#define BCM2835_NUM_ARM_SPECIFIC_IRQS   8

/* Total number of IRQs on this hardware.  */
#define BCM2835_NUM_IRQS (BCM2835_NUM_GPU_SHARED_IRQS + BCM2835_NUM_ARM_SPECIFIC_IRQS)


/* System timer - one IRQ line per output compare register.  */
#define IRQ_SYSTEM_TIMER_0 0
#define IRQ_SYSTEM_TIMER_1 1
#define IRQ_SYSTEM_TIMER_2 2
#define IRQ_SYSTEM_TIMER_3 3

/* Timer IRQ to use by default.  Note: this only be either 1 or 3, since 0 and 2
 * are already used by the VideoCore.  */
#define IRQ_TIMER          IRQ_SYSTEM_TIMER_3

/* Synopsys DesignWare Hi-Speed USB 2.0 On-The-Go Controller  */
#define IRQ_USB            9

#define IRQ_AUX            29

/* PCM sound  */
#define IRQ_PCM            55

/* PL011 UART  */
#define IRQ_PL011          57

/* SD card host controller  */
#define IRQ_SD             62

#define IRQ_BASIC          64
#define IRQ_TIMER_ARM      IRQ_BASIC + 0

#endif

