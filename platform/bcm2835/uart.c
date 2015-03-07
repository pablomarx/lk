#include <debug.h>
#include <reg.h>
#include <dev/uart.h>
#include <platform/bcm2835.h>
#include <target/debugconfig.h>

#define AUX_BASE PL011_REGS_BASE

#define AUX_ENABLES     (AUX_BASE + 0x04)
#define AUX_MU_IO_REG   (AUX_BASE + 0x40)
#define AUX_MU_IER_REG  (AUX_BASE + 0x44)
#define AUX_MU_IIR_REG  (AUX_BASE + 0x48)
#define AUX_MU_LCR_REG  (AUX_BASE + 0x4C)
#define AUX_MU_MCR_REG  (AUX_BASE + 0x50)
#define AUX_MU_LSR_REG  (AUX_BASE + 0x54)
#define AUX_MU_MSR_REG  (AUX_BASE + 0x58)
#define AUX_MU_SCRATCH  (AUX_BASE + 0x5C)
#define AUX_MU_CNTL_REG (AUX_BASE + 0x60)
#define AUX_MU_STAT_REG (AUX_BASE + 0x64)
#define AUX_MU_BAUD_REG (AUX_BASE + 0x68)

#define GPCLR0  (GPIO_REGS_BASE + 0x28)
#define GPSET0  (GPIO_REGS_BASE + 0x1C)
#define GPFSEL1 (GPIO_REGS_BASE + 0x04)

#define GPFSEL1 (GPIO_REGS_BASE + 0x04)
#define GPSET0  (GPIO_REGS_BASE + 0x1C)
#define GPCLR0  (GPIO_REGS_BASE + 0x28)
#define GPPUD       (GPIO_REGS_BASE + 0x94)
#define GPPUDCLK0   (GPIO_REGS_BASE + 0x98)

int uart_putc(int port, char c) {
	while (!(readl(AUX_MU_LSR_REG)&0x20))
		;

    writel(c, AUX_MU_IO_REG);
	return 0;
}

int uart_getc(int port, bool wait) {
	if (wait) {
    	while(!(readl(AUX_MU_LSR_REG)&0x01))
			;
    }
	else {
    	if(!(readl(AUX_MU_LSR_REG)&0x01))
			return -1;
	}
    return(readl(AUX_MU_IO_REG)&0xFF);
}

void uart_flush_tx(int port) {
    while(1)
    {
        if((readl(AUX_MU_LSR_REG)&0x100)==0) break;
    }
}

void uart_flush_rx(int port) {
    while(1)
    {
        if((readl(AUX_MU_LSR_REG)&0x100)==0) break;
    }
}

#define CR_RTSEN (1<<14)
#define CR_CTSEN (1<<15)

void uart_init_early(void)
{
    unsigned int ra;

    writel(1, AUX_ENABLES);
    writel(0, AUX_MU_IER_REG);
    writel(0, AUX_MU_CNTL_REG);
    writel(3, AUX_MU_LCR_REG);
    writel(0, AUX_MU_MCR_REG);
    writel(0, AUX_MU_IER_REG);
    writel(0xC6, AUX_MU_IIR_REG);
    writel(270, AUX_MU_BAUD_REG);
    ra=readl(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    writel(ra, GPFSEL1);
    writel(0, GPPUD);
	
    for(ra=0;ra<150;ra++) {};
    writel(CR_RTSEN|CR_CTSEN, GPPUDCLK0);

    for(ra=0;ra<150;ra++) {};
    writel(0, GPPUDCLK0);

    writel(3, AUX_MU_CNTL_REG);
}

void uart_init(void)
{

}
