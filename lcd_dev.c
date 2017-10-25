#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

#include "lcd_dev.h"
#include "lcd.h"

static int lcd_major = LCD_MAJOR;

module_param(lcd_major, int, S_IRUGO);

struct cdev cdev;

int lcd_open(struct inode * inode,struct file *filp);
int lcd_release(struct inode * inode, struct file *filp);

int lcd_open(struct inode * inode,struct file *filp)
{
	return 0;
}

int lcd_release(struct inode * inode, struct file *filp)
{
	return 0;
}

static ssize_t lcd_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
	int ret = 0;

	printk("----- misc read-----\n");
	
	printk("----- misc read out-----\n");
	return ret;
}

static ssize_t lcd_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{

	int ret = 0;


	return ret;
}

static loff_t lcd_llseek(struct file *filp, loff_t offset, int whence)
{
	return 0;
}

long lcd_ioctl(struct file*filp,unsigned int cmd,unsigned long arg)
{

	switch(cmd)
	{
		case LCD_ON : break;
		case LCD_OFF : break;
		default:
			return -EINVAL;
	}

	return 0;
}
static const struct file_operations lcd_fops =
{
	.owner = THIS_MODULE,
	.llseek = lcd_llseek,
	.read = lcd_read,
	.write = lcd_write,
	.open = lcd_open,
	.release = lcd_release,
	.unlocked_ioctl = lcd_ioctl,
};
struct miscdevice misc_dev =
{
	.minor = 50,
	.fops = &lcd_fops,
	.name = "msic_lcd"
};

static int lcd_init(void)
{
	int result;

	printk("----- misc test init-----\n");
	result = misc_register(&misc_dev);




	printk("lcd device installed\n");
	return 0;

}

static void lcd_exit(void)
{
	/*取消映射*/
	//iounmap(bcm2835_gpio);
	LCD_unInit();
	/*注销设备号*/
	misc_deregister(&misc_dev);

	printk("lcd device uninstalled\n");
}

MODULE_AUTHOR("Spice");
MODULE_LICENSE("GPL");

module_init(lcd_init);
module_exit(lcd_exit);
