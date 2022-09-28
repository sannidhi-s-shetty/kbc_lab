//Alpha-numeric LCD Interface (4Lines,20characters)
//Connected in 4bit nibble mode
//LCD handshaking:RS->P0.20,EN->P0.25 ,R/W -Gnd
//LCD data:D4,D5,D6,D7 -> P0.16,P0.17,P0.18,P0.19
#include <lpc214x.h>
#include "beep.c"
#include "ss.c"
#include "lcd.c"
#define PLOCK 0x00000400
#define LED_OFF (IO0SET = 1U << 31)
#define LED_ON (IO0CLR = 1U << 31)
#define RS_ON (IO0SET = 1U << 20)
#define RS_OFF (IO0CLR = 1U << 20)
#define EN_ON (IO1SET = 1U << 25)
#define EN_OFF (IO1CLR = 1U << 25)
#define SW4 (IO1PIN & (1<<18))
#define SW5 (IO1PIN & (1<<19))
#define SW6 (IO1PIN & (1<<20))
void SystemInit(void);
void beep4(void);
void beep5(void);
void delayHW(void) {
	T0PR = 60000-1; // produces 1msec delay 
	T0MR0 = 300; // produces 5000 x 1msec delay = 5seconds 
	T0TCR = 1; // Start the timer 
	while (!(T0TC == T0MR0) && (SW2 && SW3 && SW4)); // wait for TC to reach MR0 
	T0TCR = 2;
// Stop the timer, reset the TC,PC 
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
for(int i = 1; i<=5; i++){
	question_no(i);
	delayHW();
	if(i==1 && !SW2) ans=1;
	else if(i==2 && !SW3) ans=1;
	else if(i==3 && !SW3) ans=1;
	else if(i==4 && !SW4) ans=1;
	else if(i==5 && !SW2) ans=1;
	else{
		beep5();
		break;
	}
	//elevator();
	beep4();
	sevenseg(ans);
	ans = 0;
	if(i!=5)
		clr_lcd();
}
while(1);
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
