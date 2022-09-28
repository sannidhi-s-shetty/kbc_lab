#include <lpc214x.h>
#include <stdio.h>
#include <stdlib.h>
#define PLOCK 0x00000400
#define LED_OFF (IO0SET = 1U << 31)
#define LED_ON (IO0CLR = 1U << 31)
#define RS_ON (IO0SET = 1U << 20)
#define RS_OFF (IO0CLR = 1U << 20)
#define EN_ON (IO1SET = 1U << 25)
#define EN_OFF (IO1CLR = 1U << 25)
#define SW2 (IO0PIN & (1 << 14))
#define SW3 (IO0PIN & (1 << 15))
#define SW4 (IO1PIN & (1<<18))
#define SW5 (IO1PIN & (1<<19))
#define SW6 (IO1PIN & (1<<20))
char str[10]="5000";
void SystemInit(void);
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
void delay_ms(unsigned int j);
void SystemInit(void);
unsigned char getAlphaCode(unsigned char alphachar);
void alphadisp7SEG(char *buf);
void PWM_Init(void)
{  
    PINSEL0 |= 2 << 18 | 2 << 16 ;
   //SELECT P0.8 PWM4 AND P0.9PWM6 AS 2ND OPTION FOR PWM OPERATION
    PINSEL1 |= 1 << 10; //SELECT P0.21 PWM5 AS OPTION 1 FOR PWM OPERATION
    PWMPCR =  (1 << 12 | 1 << 13 | 1 << 14); //Enable PWM4,PWM5 and PWM        
    PWMMR0 = 100;    // load the value to MR0 to fix the pulse rate     

    PWMTCR = 0x00000009;   // bit D3 = 1 (enable  PWM), bit D0=1 (start the timer)    
}
void red(void){

			PWM_Init();        
      PWMMR4 = 0; PWMMR5 = 100; PWMMR6 = 100;        
      PWMLER = 0X70; // to enable copy to Match registers from shadow regs. 1110000
      delay_ms(100);
}

void green(void){
			PWM_Init();        
	    PWMMR4 = 100;  PWMMR5 = 100;      PWMMR6 = 0;
      PWMLER = 0X70; //  to enable copy to Match registers from shadow regs. 1110000
      delay_ms(100);
}

int doubleString(char* str)
{
	int num;
	num=atoi(str);
	num=num*2;
	
	return num;
	
}
void sevenseg(int flag)
{
    int num;

 IO0DIR |= 1U << 31 | 1U << 19 | 1U << 20 | 1U << 30 ; // to set as o/ps
 LED_ON; // make D7 Led on .. just indicate the program is running
 SystemInit();
 if(flag==1)
 {
		alphadisp7SEG(str);
    num=doubleString(str);
    sprintf(str, "%d", num);
		delay_ms(10);
 }
	//str1[] = "5000";

}

unsigned char getAlphaCode(unsigned char alphachar)
{
switch (alphachar)
{
// dp g f e d c b a - common anode: 0 segment on, 1 segment off
case '0':return 0xc0;
case '1':return 0xf9;
case '2':return 0xa4;
case '4':return 0x99; // 1000 0110
case '5':return 0x92;
case ' ':return 0xff;
//simmilarly add for other digit/characters
default : break;
}
return 0xff;
}
void alphadisp7SEG(char *buf)
{
unsigned char i,j;
unsigned char seg7_data,temp=0;
for(i=0;i<5;i++) // because only 5 seven segment digits are present
 {
seg7_data = getAlphaCode(*(buf+i)); //instead of this look up table can be used
 //to shift the segment data(8bits)to the hardware (shift registers) using
for (j=0 ; j<8; j++)
{
//get one bit of data for serial sending
temp = seg7_data & 0x80; // shift data from Most significan bit (D7)
if(temp == 0x80)
IOSET0 |= 1 << 19; //IOSET0 | 0x00080000;
else
IOCLR0 |= 1 << 19; //IOCLR0 | 0x00080000;
//send one clock pulse
IOSET0 |= 1 << 20; //IOSET0 | 0x00100000;
 delay_ms(1);
IOCLR0 |= 1 << 20; //IOCLR0 | 0x00100000;
 seg7_data = seg7_data << 1; // get next bit into D7 position
}
 }
 // send the strobe signal
IOSET0 |= 1 << 30; //IOSET0 | 0x40000000;
delay_ms(1); //nop();
IOCLR0 |= 1 << 30; //IOCLR0 | 0x40000000;
return;
 }

void LCD_DisplayString(const char *ptr_stringPointer_u8);
//const char *q[3] = { "Sun rises in?",
//                                "Sun sets in?",
//                                "Earth shape is?" };

//const char *a[3][4] = { {"a) east" , "b) west", "c) north", "d) south"}, 
//                    {"a) east" , "b) west", "c) north", "d) south"},
//                     {"a) sqaure", "b) giode", "c) sphere", "d) triangle"} };
//const char* Q(int i){
//    return(q[i]);
//}

void question_no(int ch){
	SystemInit();
IO0DIR |= 1U << 31 | 0x00FF0000 ; // to set P0.16 to P0.23 as o/ps
IO1DIR |= 1U << 25; // to set P1.25 as o/p used for EN
 // make D7 Led on off for testing
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
LCD_CmdWrite(0x80); LCD_DisplayString("Q3. Earth shape is?");
LCD_CmdWrite(0xc0); LCD_DisplayString("a) square");
LCD_CmdWrite(0x94); LCD_DisplayString("b) sphere");
LCD_CmdWrite(0xd4); LCD_DisplayString("c) triangle");
	
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

void beep4(void){
	PINSEL1 = 0x00080000;
	IO0DIR |= 1U<<31 | 0xFF<<16;
	short int val = 0;
			while(val!=50){
				DACR = (1<<16) | (val<<6);
				val++;
			}
			while(val!=0){
				DACR = (1<<16) | (val<<6);
				val--;
			}
}
void beep5(void){
	PINSEL1 = 0x00080000;
	IO0DIR |= 1U<<31 | 0xFF<<16;
	short int val = 0;

			while(val!=1023){
				DACR = (1<<16) | (val<<6);
				val++;
			}
}
void beep6(void){
	PINSEL1 = 0x00080000;

	IO0DIR |= 1U<<31 | 0xFF<<16;
	short int val = 1023;
			DACR = (1<<16) | (val<<6);
			delay_ms(1);
			val = 0;
			DACR = (1<<16) | (val<<6);
			delay_ms(1);
}
void delayHW(void) {
	T0PR = 60000-1; // produces 1msec delay 
	T0MR0 = 300; // produces 5000 x 1msec delay = 5seconds 
	T0TCR = 1; // Start the timer 
	while (!(T0TC == T0MR0) && (SW2 && SW3 && SW4)); // wait for TC to reach MR0 
	T0TCR = 2;
// Stop the timer, reset the TC,PC 
}
void clr_lcd(){
	LCD_CmdWrite(0x80); LCD_DisplayString("                    ");
LCD_CmdWrite(0xc0); LCD_DisplayString("                   ");
LCD_CmdWrite(0x94); LCD_DisplayString("                   ");
LCD_CmdWrite(0xd4); LCD_DisplayString("                   ");
}
int main()
{
SystemInit();
IO0DIR |= 1U << 31 | 0x00FF0000 ; // to set P0.16 to P0.23 as o/ps
IO1DIR |= 1U << 25; // to set P1.25 as o/p used for EN
 // make D7 Led on off for testing
LED_ON; delay_ms(500);LED_OFF;delay_ms(500);
LCD_Reset();
LCD_Init();
delay_ms(100);
int ans = 0;
int flag = 0;
for(int i = 1; i<=5; i++){
	
//	question_no(i);
//	//delayHW();
//	while(!flag){
//	if(i==1 && !SW2){ ans=1; break;}
//	if(i==2 && !SW3) { ans=1; break;}
//	if(i==3 && !SW3) { ans=1; break;}
//	if(i==4 && !SW4) { ans=1; break;}
//	if(i==5 && !SW2) { ans=1; break;}
//	if(!SW2 || !SW3 || !SW4) flag =1;
//	}
//	if(ans!=1){
//		beep5();
//		break;
//	}
//	//elevator();
	sevenseg(1);
	beep4();
	ans = 0;
	flag=0;
	if(i!=5)
		clr_lcd();
}
//while(1);
}


void SystemInit(void)
{
 PLL0CON = 0x01;
 PLL0CFG = 0x24;
 PLL0FEED = 0xAA;
 PLL0FEED = 0x55;
 while( !( PLL0STAT & PLOCK ))
 { ; }
 PLL0CON = 0x03;
 PLL0FEED = 0xAA; // lock the PLL registers after setting the required PLL
 PLL0FEED = 0x55;
 VPBDIV = 0x01; // PCLK is same as CCLK i.e 60Mhz
}
void delay_ms(unsigned int j)
{
unsigned int x,i;
 for(i=0;i<j;i++)
 {
 for(x=0; x<10000; x++);
 }
}
