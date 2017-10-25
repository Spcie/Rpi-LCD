ifneq ($(KERNELRELEASE),)

obj-m := LCD_Dev.o
OLED_Dev-objs := ./hardware/hw_gpio.o ./hardware/hw_spi.o ./hardware/hw_delay.o olcd.o lcd_dev.o ./sim/soft_ispi.o ./font/font.o

else


KDIR := /lib/modules/4.9.44-v7+/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	rm -f *.ko *.o *.mod.o *mod.c *.symvers *.order

endif
