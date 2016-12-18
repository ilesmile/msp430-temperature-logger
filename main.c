#include "main.h"
unsigned char data[9];
unsigned int result[3]; 
unsigned char resolution = 0;
void main()
{
	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ; 
	DCOCTL = CALDCO_1MHZ;
	
	Setup_LCD();
	Clr_LCD();
	Move_LCD(1, 3);
	Print_LCD("Nhiet do la: ");
	
	P2SEL &= ~BIT6;
	P2DIR &= ~BIT6;
	P2OUT |= BIT6;
	P2REN |= BIT6; 
	P2IE |= BIT6;
	P2IES &= ~BIT6;
	P2IFG &= ~BIT6;
	
	DS18B20_Configuration_Register(resolution);
	
	__bis_SR_register(GIE);
	
	while(1)
	{		
		DS18B20_Send_Function_Command(0x44);	//send T-convert command
		DS18B20_Read_Multi_Byte(data, 9);		//store data in RAM msp430
		DS18B20_Temperature_Format_Transfer(data, result);
	
		//-----------Display temperature----------
		Move_LCD(2, 1);
		Display_Temperature(result);
	}
	
}
#pragma vector = PORT2_VECTOR
interrupt void PORT2_ISR(void)
{
	if(++resolution == 4)
		resolution = 0;
		
	DS18B20_Configuration_Register(resolution);
	
	P2IFG &= ~BIT6;
}
