#ifndef __SOFT_SPI_H
#define __SOFT_SPI_H

#define SPI_MOD_COMMAND	(0x00)
#define SPI_MOD_DATA	(0x01)

void soft_spi_init(volatile unsigned int*  peripherals_base);
void soft_spi_setCE(unsigned char ce);
void soft_spi_setDC(unsigned char dc);
void soft_spi_rest(unsigned char rest);
void soft_spi_wirteBus(unsigned char byte);
void soft_spi_delayMs(unsigned int ms);
void soft_spi_delayUs(unsigned int us);
void soft_spi_uninit(void);

#endif
