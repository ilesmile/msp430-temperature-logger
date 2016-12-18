#ifndef PORT_H_
#define PORT_H_
//CONFIG PINS FOR DS18B20_1
#define DQ 				BIT3 
#define _1_WIRE_DIR		P1DIR
#define _1_WIRE_IN		P1IN
#define _1_WIRE_OUT		P1OUT
//CONFIG PINS FOR DS18B20_2
#define DQ_2			BIT7 
#define _1_WIRE_DIR_2	P2DIR
#define _1_WIRE_IN_2	P2IN
#define _1_WIRE_OUT_2	P2OUT
//CONFIG PINS FOR LCD
#define DATA_O         P1OUT
#define CTRL_DDR       P1DIR
#define CTRL           P1OUT    
#define DATA_I         P1IN
#define RS             0      //PB0
#define RW             1      //PB1
#define EN             2      //PB2
#define sbi(port, bit) port |=  (1<<bit)
#define cbi(port, bit) port &= ~(1<<bit)
// RS = 0: chon thanh ghi lenh, RS = 1:thanh ghi du lieu
// RW = 0: ghi du lieu,       RW = 1: doc du lieu
//   0x28: giao tiep 4 bit, 2 dong voi font 5x8
#endif /*PORT_H_*/
