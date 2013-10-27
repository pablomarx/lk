#include <debug.h>
#include <reg.h>
#include <dev/uart.h>
#include <platform/bcm2835.h>
#include <target/debugconfig.h>

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

#define GPCLR0  0x20200028
#define GPSET0  0x2020001C
#define GPFSEL1 0x20200004

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

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
    writel((1<<14)|(1<<15), GPPUDCLK0);
    for(ra=0;ra<150;ra++) {};
    writel(0, GPPUDCLK0);
    writel(3, AUX_MU_CNTL_REG);
}

void uart_init(void)
{

}
