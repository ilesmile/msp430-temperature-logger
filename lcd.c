#include "main.h"
// Functions for communication to lCD
void Write_LCD(unsigned char ch)
{
   unsigned char temp_mask, HNib, LNib;
   temp_mask = 0x0F & CTRL;      //giu lai cac bit dieu  khien
   HNib = ch & 0xF0;
   LNib = ch << 4;
   //ghi 4 bit cao
   DATA_O = HNib | temp_mask;
   sbi(CTRL, EN);
   cbi(CTRL, EN);
   //ghi 4 bit thap
   DATA_O = LNib | temp_mask;
   sbi(CTRL, EN);
   cbi(CTRL, EN);
}
unsigned char Read_LCD()
{
   unsigned char HNib, LNib;
   CTRL_DDR = 0x07 | (CTRL_DDR & 0x08);
   //doc 4 bit cao
   cbi(CTRL, EN);
   sbi(CTRL, EN);
   HNib = DATA_I & 0xF0;
   //doc 4 bit thap
   cbi(CTRL, EN);
   sbi(CTRL, EN);
   LNib = DATA_I>>4;
   return (HNib | LNib);
}
void Wait_LCD()
{
   unsigned char check_busy_flag;
   cbi(CTRL, RS);
   sbi(CTRL, RW);
   while(1)
   {
      check_busy_flag = (1<<7) & Read_LCD();
      if(check_busy_flag == 0x00) break;
   }
//   _delay_ms(2);
   cbi(CTRL, RW);
   CTRL_DDR = 0xF7;
}
void Clr_LCD()
{
   cbi(CTRL, RS);
   cbi(CTRL, RW);
   Write_LCD(0x01);
   Wait_LCD();
}
void Setup_LCD()
{
   CTRL_DDR |= 0xF7;
//   PORTB = 0xFF;
   //dat che do: 4 bit-mode
   cbi(CTRL, RS);         //chon thanh ghi lenh
   cbi(CTRL, RW);         //che do ghi
   Write_LCD(0x02);      //4 bit - mode
   Wait_LCD();
   //dat che do: 2 line, font 5x8
   Write_LCD(0x28);      
   Wait_LCD();
   //bat hien thi
   Write_LCD(0x0C);
   Wait_LCD();

   Write_LCD(0x06);
   Wait_LCD();
}
void Putchar_LCD(unsigned char ch)
{
   sbi(CTRL, RS);
   cbi(CTRL, RW);
   Write_LCD(ch);
   Wait_LCD();
}
void Print_LCD(char* str)
{
   unsigned char i = 0;
   while(str[i]!='\0')
   {
      Putchar_LCD(str[i++]);   
   }
}
void Move_LCD(unsigned char y, unsigned char x)
{
   unsigned char add;
   add = 64*(y - 1) + (x - 1) + 0x80;
   cbi(CTRL, RS);
   cbi(CTRL, RW);
   Write_LCD(add);
   Wait_LCD();
}

