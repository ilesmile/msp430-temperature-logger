/********************************************************
this work is based on original work of  hoanglx123 at github/hoanglx123/How-to-measure-temperature-with-DS18B20-and-MSP430

Author: Ilija Pecelj

Description: 
********************************************************/
#include "main.h"
extern unsigned char data[9];
unsigned char Reset_1_Wire()
{
	unsigned char status;
	bis(_1_WIRE_DIR, DQ);
	bic(_1_WIRE_OUT, DQ);
	__delay_cycles(480);	//obtain about 480us
	bic(_1_WIRE_DIR, DQ);	//release about 60us
	__delay_cycles(60);		//60us
	status = check_bit(_1_WIRE_IN, DQ); //DS18B20 response by pulling DQ down, status = 0
	__delay_cycles(420);	//420 us, wait to cycle end
	
	return status;
}
void Write_1_Byte_1_Wire(unsigned char data)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		//LSB is sent first
		if(check_bit(data, BIT0))
		{
			//master write "1" slot
			bis(_1_WIRE_DIR, DQ);
			bic(_1_WIRE_OUT, DQ);
			__delay_cycles(1);		//obtain about 10us
			bic(_1_WIRE_DIR, DQ);
			__delay_cycles(60);		//delay 60us to wait cycle end
		}
		else 
		{
			//master write "0" slot
			bis(_1_WIRE_DIR, DQ);
			bic(_1_WIRE_OUT, DQ);
			__delay_cycles(60);		//obtain about 60us
			bic(_1_WIRE_DIR, DQ);
			__delay_cycles(1);		//15us recovery between individual write slots
		}
		//shift right 1 bit
		data = data >> 1;
	}
}
unsigned char Read_1_Byte_1_Wire()
{
	unsigned char data;
	unsigned char i;
	
	for(i = 0; i < 8; i++)
	{
		bis(_1_WIRE_DIR, DQ);
		bic(_1_WIRE_OUT, DQ);
	//	__delay_cycles(1);		//obtain about 10us
		bic(_1_WIRE_DIR, DQ);
	//	__delay_cycles(1);
		
		if(check_bit(_1_WIRE_IN, DQ))
		{
			//master read "1" slot
			data = (data >> 1)|0x80;
		}
		else
		{
			//master read "0" slot
			data = (data >> 1)|0x00;
		}
		
		__delay_cycles(60);		//70us recovery time between slots
	}
	
	return data;
}
void DS18B20_Read_Multi_Byte(unsigned char *data, unsigned char number)
{
	unsigned char i;
	
	if(Reset_1_Wire() == 0)
	{
		Write_1_Byte_1_Wire(0xCC);		//send skip rom command
		Write_1_Byte_1_Wire(0xBE);		//send command read scratchpad
		
		for(i = 0; i < number; i++)
		{
			*(data + i) = Read_1_Byte_1_Wire();	
		}	
	}
}
void DS18B20_Write_Multi_Byte(unsigned char *data)
{
	if(Reset_1_Wire() == 0)
	{
		Write_1_Byte_1_Wire(0xCC);		//send skip rom command
		Write_1_Byte_1_Wire(0x4E);		//send write scratchpad command
		
		//send data
		Write_1_Byte_1_Wire(data[2]);
		Write_1_Byte_1_Wire(data[3]);
		Write_1_Byte_1_Wire(data[4]);	
	}
}
void DS18B20_Send_Function_Command(unsigned char command)
{
	if(Reset_1_Wire() == 0)
	{	
			Write_1_Byte_1_Wire(0xCC);		//send skip rom command
			Write_1_Byte_1_Wire(command);		//send command convert temperature
			if(command == 0x44)
				while(!check_bit(_1_WIRE_IN, DQ)); //check converting temperature complete
	}
}
void DS18B20_Temperature_Format_Transfer(unsigned char *data, unsigned int *result)
{
	unsigned char Res;				//Thermometer Resolution Configuration
	unsigned int temp;
	Res = (*(data + 4))>>5;	//get value R0, R1 in Configuration Register		
	
	//get sign 
	if(check_bit(*(data + 1), BIT7))
	{
		*(result + 0) = 0xFF;		//result[0] = sign: 0x00 is +, 0xFF is -
	}
	else 
	{
		*(result + 0) = 0x00;
	}
	
	//get value
	switch(Res)
	{
		case 0x00://9 bits resolution
			*(result + 1) = (*(data + 0)>>4)|((*(data + 1)&0x07)<<4);
			*(result + 2) = ((*(data + 0)&0x08)>>3)*5;
			break;
		case 0x01://10 bits resolution
			*(result + 1) = (*(data + 0)>>4)|((*(data + 1)&0x07)<<4);
			*(result + 2) = ((*(data + 0)&0x0C)>>2)*25;
			break;
		case 0x02://11 bits resolution
			*(result + 1) = (*(data + 0)>>4)|((*(data + 1)&0x07)<<4);
			*(result + 2) = ((*(data + 0)&0x0E)>>1)*125;
			break;
		case 0x03://12 bits resolution
			*(result + 1) = (*(data + 0)>>4)|((*(data + 1)&0x07)<<4) ;
			temp = (unsigned int)(*(data + 0)&0x0F)*625;
			*(result + 2) = (unsigned char)(temp/100);
			break;
	}
}
void DS18B20_Configuration_Register(unsigned char temp)
{
	DS18B20_Read_Multi_Byte(data, 9);		//luu du lieu trong DS18B20 vao RAM msp430
	
	data[4] = temp<<5;
	DS18B20_Write_Multi_Byte(data);
}
void HEX_2_LCD(unsigned int hex)
{
	unsigned char temp[4];
	unsigned char i = 0; 
	
	while(1)
	{
		temp[i] = hex%10;
		hex = hex/10;
		if(hex == 0)
			break; 
		i++;
	}
	while(1)
	{
		Putchar_LCD(temp[i]|0x30);
		if(i == 0)
			break;
		i--;
	}
}
void Display_Temperature(unsigned int* result)
{
	//display sign
	if(result[0] == 0x00)
	{
		Putchar_LCD('+');
	}
	else 
	{
		Putchar_LCD('-');
	}
	//display Int
	HEX_2_LCD(result[1]);
	//Display dot
	Putchar_LCD('.');
	//display Decimal
	HEX_2_LCD(result[2]);
	
	Putchar_LCD(0xDF);
	Print_LCD("C       ");
}
