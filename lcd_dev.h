#ifndef __LCD_DEV_H
#define __LCD_DEV_H

#ifndef LCD_MAJOR
#define LCD_MAJOR 124	//混杂设备可不用
#endif

#ifndef LCD_NR_DEVS
#define LCD_NR_DEVS 2	//可能为1更好
#endif


/*定义幻数*/
#define LCD_IOC_MAGIC 'x'

/*定义命令*/
#define LCD_ON _IO(LCD_IOC_MAGIC,1)
#define LCD_OFF _IO(LCD_IOC_MAGIC,2)


#endif /*__IOCTRL_H*/
