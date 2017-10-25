#ifndef __LCD_H
#define __LCD_H

int LCD_Init(void);
void LCD_unInit(void);
void LCD_Clear(unsigned int Color);
void LCD_SetPointColor(unsigned int color);
void LCD_SetBackColor(unsigned int color);
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int Color);
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);
void LCD_ClearLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);
void LCD_DrawCircle(unsigned int x0,unsigned int y0,unsigned int r,unsigned int color);
void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned char size,unsigned char mode);
void LCD_ShowString(unsigned int x,unsigned int y,unsigned int width,unsigned int height,unsigned char size,unsigned char *p);
void number(unsigned int x,unsigned int y,const unsigned char number);
void LCD_ShowNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned char size);

#endif
