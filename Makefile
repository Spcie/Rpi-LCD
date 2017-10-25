ifneq ($(KERNELRELEASE),)

obj-m := LCD_Dev.o
LCD_Dev-objs := ./hardware/hw_gpio.o ./hardware/hw_spi.o ./hardware/hw_delay.o lcd.o lcd_dev.o ./sim/soft_spi.o ./font/font.o

else


KDIR := /lib/modules/4.9.44-v7+/build

all:
	make -C $(KDIR) M=$(shell pwd) modules 
clean:
	rm -f *.ko *.o *.mod.o *mod.c *.symvers *.order

endif
