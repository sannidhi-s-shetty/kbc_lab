//Seven Segment Display Program:
//P0.19 Data pin of 1st shift register
//P0.20 Clock pin of shift registers, make 1 to 0
//P0.30 Strobe pin of shift registers: 1 to 0
#include <lpc214x.h>
#include <stdio.h>
#include <stdlib.h>
#define LED_OFF (IO0SET = 1U << 31)
#define LED_ON (IO0CLR = 1U << 31)
#define PLOCK 0x00000400
#define SW2 (IO0PIN & (1 << 14))
#define SW3 (IO0PIN & (1 << 15))

void delay_ms(unsigned int j);
void SystemInit(void);
unsigned char getAlphaCode(unsigned char alphachar);
void alphadisp7SEG(char *buf);
int doubleString(char* str)
{
	int num;
	num=atoi(str);
	num=num*2;
	
	return num;
	
}
void sevenseg(int flag)
{
	char str[10]="5000";
    int num;

 IO0DIR |= 1U << 31 | 1U << 19 | 1U << 20 | 1U << 30 ; // to set as o/ps
 LED_ON; // make D7 Led on .. just indicate the program is running
 SystemInit();
 if(flag==0)
 {
		alphadisp7SEG(str);
    num=doubleString(str);
    sprintf(str, "%d", num);
		delay_ms(1000);
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