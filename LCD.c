#include "stdint.h"
#include "D:/Keil/EE319Kware/inc/tm4c123gh6pm.h"

void SystemInit(){}


void LCD_command(unsigned char com)
{	
	delay(50);
	GPIO_PORTA_DATA_R &=~0x20; //Rs and RW are 0
	delay(2);
	GPIO_PORTB_DATA_R = com;
	delay(2);
	GPIO_PORTA_DATA_R |= 0x80;
	delay(2);
	GPIO_PORTA_DATA_R &=~0x80; 
	delay(2);
}

void LCD_DATA(unsigned char data)
{
	SYSCTL_RCGCGPIO_R |= 0x00000020; 
	GPIO_PORTA_DATA_R |= 0x20; // Make RS = 1 and RW =0     /* 0010_0000 A5
	GPIO_PORTB_DATA_R = data; // take the data
	GPIO_PORTA_DATA_R |= 0x80; //enable E                   /*   10000000 A7 
	delay(2);
	GPIO_PORTA_DATA_R &=~0x80;    // Disable E
	delay(2);
	GPIO_PORTF_DATA_R = 0x02 ;
	delay(2);
}

void init(void)
{
	delay(50); 
	SYSCTL_RCGCGPIO_R |= 0x01; //init Port A&B clk
	SYSCTL_RCGCGPIO_R |= 0x02; //init Port A&B clk
//For Port B
	delay(50) ;
	GPIO_PORTB_LOCK_R=0x4C4F434B;
	GPIO_PORTB_CR_R = 0xFF; // FOR ALL PINS I USE
	GPIO_PORTB_DIR_R=0xFF; // to enable all the pins in port B
	GPIO_PORTB_DEN_R=0XFF;// to make them degital
	GPIO_PORTB_AFSEL_R =0; // neednt
	GPIO_PORTB_PCTL_R = 0; //neednt
	GPIO_PORTB_AMSEL_R = 0; //neednt
	GPIO_PORTB_PUR_R = 0 ;
//For Port A
	//GPIO_PORTA	_CR_R = 0x1F; // FOR ALL PINS I USE
	GPIO_PORTA_LOCK_R=0x4C4F434B;
	GPIO_PORTA_CR_R = 0xF0; // FOR ALL PINS I USE
	GPIO_PORTA_DIR_R=0xE0;
	GPIO_PORTA_DEN_R=0xE0;
	GPIO_PORTA_AFSEL_R =0; // neednt
	GPIO_PORTA_PCTL_R = 0; //neednt
	GPIO_PORTA_AMSEL_R = 0; //neednt
	GPIO_PORTA_PUR_R = 0 ;
	delay(2);
	//LCD_command(0x03);	
	delay(2);
	LCD_command(0x06);	/* Increment cursor (shift cursor to right) */
	delay(2);
	LCD_command(0x0E);
	delay(2);
	LCD_command(0x38);	/* 2 line, 5*7 matrix in 4-bit mode */
	delay(2);
	LCD_command(0x01);	/* Clear display screen */
	delay(2);
}
int main()
{
	init();
	init_portF() ;
		char dist[]={'D','I','S','T','A','N','C','E','='};
	  
		LCD_command(0x80);
	
	LCD_displayString(dist);
	Exceed_distance(150);
		
}
