#ifndef LCD_H_
#define LCD_H_
void Write_LCD(unsigned char ch);
unsigned char Read_LCD();
void Wait_LCD();
void Clr_LCD();
void Setup_LCD();
void Putchar_LCD(unsigned char ch);
void Print_LCD(char* str);
void Move_LCD(unsigned char y, unsigned char x);
void Cursor_On_LCD();
void Cursor_Off_LCD();
#endif /*LCD_H_*/
