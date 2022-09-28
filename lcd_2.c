//Alpha-numeric LCD Interface (4Lines,20characters)
//Connected in 4bit nibble mode
//LCD handshaking:RS->P0.20,EN->P0.25 ,R/W -Gnd
//LCD data:D4,D5,D6,D7 -> P0.16,P0.17,P0.18,P0.19
#include <lpc214x.h>
#include "beep.c"
#include "ss.c"
#define PLOCK 0x00000400
#define LED_OFF (IO0SET = 1U << 31)
#define LED_ON (IO0CLR = 1U << 31)
#define RS_ON (IO0SET = 1U << 20)
#define RS_OFF (IO0CLR = 1U << 20)
#define EN_ON (IO1SET = 1U << 25)
#define EN_OFF (IO1CLR = 1U << 25)

void SystemInit(void);
static void delay_ms(unsigned int j);//millisecond delay
static void delay_us(unsigned int count);//microsecond delay
static void LCD_SendCmdSignals(void);
static void LCD_SendDataSignals(void);
static void LCD_SendHigherNibble(unsigned char dataByte);
static void LCD_CmdWrite( unsigned char cmdByte);
static void LCD_DataWrite( unsigned char dataByte);
static void LCD_Reset(void);
static void LCD_Init(void);
void LCD_DisplayString(const char *ptr_stringPointer_u8);


void clr_lcd(){
	LCD_CmdWrite(0x80); LCD_DisplayString("                    ");
LCD_CmdWrite(0xc0); LCD_DisplayString("                   ");
LCD_CmdWrite(0x94); LCD_DisplayString("                   ");
LCD_CmdWrite(0xd4); LCD_DisplayString("                   ");
}

void question_no(int ch){
	SystemInit();
IO0DIR |= 1U << 31 | 0x00FF0000 ; // to set P0.16 to P0.23 as o/ps
IO1DIR |= 1U << 25; // to set P1.25 as o/p used for EN

LED_ON; delay_ms(500);LED_OFF;delay_ms(500);
LCD_Reset();
LCD_Init();
delay_ms(100);
	switch (ch){
	case 1: 
LCD_CmdWrite(0x80); LCD_DisplayString("Q1. Sun rises in?");
LCD_CmdWrite(0xc0); LCD_DisplayString("a) east");
LCD_CmdWrite(0x94); LCD_DisplayString("b) west");
LCD_CmdWrite(0xd4); LCD_DisplayString("c) north");
	
	break;
	case 2:
LCD_CmdWrite(0x80); LCD_DisplayString("Q2. Sun sets in?");
LCD_CmdWrite(0xc0); LCD_DisplayString("a) east");
LCD_CmdWrite(0x94); LCD_DisplayString("b) west");
LCD_CmdWrite(0xd4); LCD_DisplayString("c) north");
	
	break;
	case 3:
LCD_CmdWrite(0x80); LCD_DisplayString("Q3. Earth shape is?");
LCD_CmdWrite(0xc0); LCD_DisplayString("a) square");
LCD_CmdWrite(0x94); LCD_DisplayString("b) sphere");
LCD_CmdWrite(0xd4); LCD_DisplayString("c) triangle");
	
	break;
	case 4:
LCD_CmdWrite(0x80); LCD_DisplayString("Q4. New Years when?");
LCD_CmdWrite(0xc0); LCD_DisplayString("a) March 6");
LCD_CmdWrite(0x94); LCD_DisplayString("b) August 15");
LCD_CmdWrite(0xd4); LCD_DisplayString("c) January 1");
	
	break;
	case 5:
LCD_CmdWrite(0x80); LCD_DisplayString("Q5. Where is RVCE?");
LCD_CmdWrite(0xc0); LCD_DisplayString("a) Mysore Road");
LCD_CmdWrite(0x94); LCD_DisplayString("b) Mysore");
LCD_CmdWrite(0xd4); LCD_DisplayString("c) Europe");
	
	break;
	default:
		break;
}
}

static void LCD_CmdWrite( unsigned char cmdByte)
{
 LCD_SendHigherNibble(cmdByte);
 LCD_SendCmdSignals();
 cmdByte = cmdByte << 4;
 LCD_SendHigherNibble(cmdByte);
 LCD_SendCmdSignals();
}
static void LCD_DataWrite( unsigned char dataByte)
{
 LCD_SendHigherNibble(dataByte);
 LCD_SendDataSignals();
 dataByte = dataByte << 4;
 LCD_SendHigherNibble(dataByte);
 LCD_SendDataSignals();
}
static void LCD_Reset(void)
{
 /* LCD reset sequence for 4-bit mode*/
 LCD_SendHigherNibble(0x30);
 LCD_SendCmdSignals();
 delay_ms(100);
 LCD_SendHigherNibble(0x30);
 LCD_SendCmdSignals();
 delay_us(200);
 LCD_SendHigherNibble(0x30);
 LCD_SendCmdSignals();
 delay_us(200);
 LCD_SendHigherNibble(0x20);
 LCD_SendCmdSignals();
 delay_us(200);
}
static void LCD_SendHigherNibble(unsigned char dataByte)
{
 //send the D7,6,5,D4(uppernibble) to P0.16 to P0.19
 IO0CLR = 0X000F0000;IO0SET = ((dataByte >>4) & 0x0f) << 16;
}
static void LCD_SendCmdSignals(void)
{
 RS_OFF; // RS - 1
 EN_ON;delay_us(100);EN_OFF; // EN - 1 then 0
}
static void LCD_SendDataSignals(void)
{
 RS_ON;// RS - 1
 EN_ON;delay_us(100);EN_OFF; // EN - 1 then 0
}
static void LCD_Init(void)
{
delay_ms(100);
LCD_Reset();
LCD_CmdWrite(0x28u); //Initialize the LCD for 4-bit 5x7 matrix type
LCD_CmdWrite(0x0Eu); // Display ON cursor ON
LCD_CmdWrite(0x01u); //Clear the LCD
LCD_CmdWrite(0x80u); //go to First line First Position
}
void LCD_DisplayString(const char *ptr_string)
{
// Loop through the string and display char by char
 while((*ptr_string)!=0)
 LCD_DataWrite(*ptr_string++);
}
static void delay_us(unsigned int count)
{
 unsigned int j=0,i=0;
 for(j=0;j<count;j++)
 {
 for(i=0;i<10;i++);
 }
}

