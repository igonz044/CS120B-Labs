#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"
#include "bit.h" 
/*-------------------------------------------------------------------------*/

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))


/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7 		// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {  LCD_WriteCommand(0x01);
}

void LCD_Write4bit(unsigned char);		//writing the 4bit input 

void LCD_init(void) {// used some help from the youtube links as well as my TA and my partner
    delay_ms(50); 
	LCD_Write4bit(0x30);
	delay_ms(10);
	LCD_Write4bit(0x30);
	delay_ms(10);
	LCD_Write4bit(0x30);
	delay_ms(10);
	LCD_Write4bit(0x20);			
	delay_ms(10);
	//delays in for the cursor blinks 
	LCD_WriteCommand(0x28);		
	LCD_WriteCommand(0x08);	
	LCD_WriteCommand(0x01);		
	delay_ms(2);
	LCD_WriteCommand(0x06);	
	LCD_WriteCommand(0x0C);		
										
	delay_ms(10);						 
}

//overwrites just in case if using 
void LCD_Write4bit(unsigned char Command) {
	CLR_BIT(CONTROL_BUS,RS);
	DATA_BUS = Command;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(2); 
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command & 0xF0;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
      DATA_BUS = (Command << 4) & 0xF0;
      SET_BIT(CONTROL_BUS,E);
      asm("nop");
      CLR_BIT(CONTROL_BUS,E);
   delay_ms(2); 
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data & 0xF0;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
      SET_BIT(CONTROL_BUS,RS);
      DATA_BUS = (Data << 4) & 0xF0;
      SET_BIT(CONTROL_BUS,E);
      asm("nop");
      CLR_BIT(CONTROL_BUS,E);
      delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { 
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	
   }
}

void delay_ms(int miliSec) 
{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}
//custom character
void LCD_CreateCharacter(unsigned char pixel[8], unsigned char reg){
	if(reg < 8) 
		LCD_WriteCommand(0x40 + (reg * 8));
		for(int i = 0; i < 8; i++) {
				LCD_WriteData(pixel[i]);
		}
	LCD_WriteCommand(0x80);
}