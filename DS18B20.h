#ifndef DS18B20_H_
#define DS18B20_H_
#define	bis(port, bit) 			port |= bit;
#define	bic(port, bit) 			port &= ~bit
#define check_bit(port, bit) 	(port&bit)
unsigned char Reset_1_Wire();//dung de reset DS18B20 truoc khi giao tiep voi no
void Write_1_Byte_1_Wire(unsigned char data);//gui 1 byte du lieu den 18B20
unsigned char  Read_1_Byte_1_Wire();//doc du lieu tu DS18B20
void DS18B20_Send_Function_Command(unsigned char command);//gui lenh den DS18B20
void DS18B20_Read_Multi_Byte(unsigned char *data, unsigned char number);//doc nhieu byte tu DS18B20
void DS18B20_Write_Multi_Byte(unsigned char *data);//Gui nhieu byte den DS18B20
void DS18B20_Temperature_Format_Transfer(unsigned char *data, unsigned int *result);//xu ly du lieu nhan duoc tu DS18B20 thanh: dau, phan nguyen, phan thap phan
void DS18B20_Configuration_Register(unsigned char temp);//chon 9, 10, 11, hay 12 bit 
void Display_Temperature(unsigned int* result);// hien thi nhiet do ra LCD
void HEX_2_LCD(unsigned int hex);//chuyen chu so thanh ky tu
#endif /*DS18B20_H_*/
