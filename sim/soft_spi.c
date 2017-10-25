
#include "hw_gpio.h"
#include "hw_delay.h"
#include "soft_spi.h"


void soft_spi_init(volatile unsigned int*  peripherals_base)
{
    bcm_gpio_init(peripherals_base);
    bcm_gpio_fsel(RPI_GPIO_07, BCM_GPIO_FSEL_OUTP);
    bcm_gpio_fsel(RPI_GPIO_08, BCM_GPIO_FSEL_OUTP);
    bcm_gpio_fsel(RPI_GPIO_09, BCM_GPIO_FSEL_OUTP);
    bcm_gpio_fsel(RPI_GPIO_10, BCM_GPIO_FSEL_OUTP);
    bcm_gpio_fsel(RPI_GPIO_11, BCM_GPIO_FSEL_OUTP);

    bcm_gpio_fsel(RPI_GPIO_05, BCM_GPIO_FSEL_OUTP);
    bcm_gpio_fsel(RPI_GPIO_06, BCM_GPIO_FSEL_OUTP);
}

void soft_spi_setCE(unsigned char ce)
{
    // use CE0
    if(ce)
    {
        bcm_gpio_set(RPI_GPIO_08);
    }else
    {
        bcm_gpio_clr(RPI_GPIO_08);
    }
}

void soft_spi_setDC(unsigned char dc)
{
    if(dc)
    {
        bcm_gpio_set(RPI_GPIO_05);
    }else
    {
        bcm_gpio_clr(RPI_GPIO_05);
    }

}

void soft_spi_rest(unsigned char rest)
{
    if(dc)
    {
        bcm_gpio_set(RPI_GPIO_06);
    }else
    {
        bcm_gpio_clr(RPI_GPIO_06);
    }
}
static void soft_mosi_out(unsigned char lev)
{
    if(lev)
    {
        bcm_gpio_set(RPI_GPIO_10);
    }else
    {
        bcm_gpio_clr(RPI_GPIO_10);
    }
}

static void soft_sck_out(unsigned char lev)
{
    if(lev)
    {
        bcm_gpio_set(RPI_GPIO_11);
    }else
    {
        bcm_gpio_clr(RPI_GPIO_11);
    }
}

void soft_spi_wirteBus(unsigned char byte)
{
    int i;
	for(i=0;i<8;i++)
	{
		if(byte&0x80)
		{
			soft_mosi_out(1);
		}
		else
		{
			soft_mosi_out(0);
		}

		soft_sck_out(0);
		byte = byte << 1;
		soft_sck_out(1);
	}
}

void soft_spi_delayMs(unsigned int ms)
{
    bcm_st_delay_ms(ms);
}

void soft_spi_delayUs(unsigned int us)
{
    bcm_st_delay_us(us);
}

void soft_spi_uninit(void)
{
    bcm_gpio_fsel(RPI_GPIO_07, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(RPI_GPIO_08, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(RPI_GPIO_09, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(RPI_GPIO_10, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(RPI_GPIO_11, BCM_GPIO_FSEL_INPT);
    
    bcm_gpio_uninit();
}
