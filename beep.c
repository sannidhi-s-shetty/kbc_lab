#include<lpc214x.h>
#define PLOCK 0x00000400
#define LED_ON (IO0CLR = 1U<<31)
#define LED_OFF (IO0SET = 1U<<31)
#define SW2 (IO0PIN & (1<<14))
#define SW3 (IO0PIN & (1<<15))
#define SW4 (IO1PIN & (1<<18))
#define SW5 (IO1PIN & (1<<19))
#define SW6 (IO1PIN & (1<<20))
void delay_ms(unsigned int j);
void SystemInit(void);
void beep4();
void beep5();
void beep6();
unsigned int ans = 0; // 0 - for wrong ans, 1 - for right answer
int main(){
	short int i=0,val;
	PINSEL1 = 0x00080000;
	IO0DIR |= 1U<<31 | 0xFF<<16;  //P0.25 is output for 
	SystemInit();
	while(1){	
		if(ans){  // triangular
			beep4();
		}
		else {
			beep5();
		}
		//else if(!SW6){
		//	beep6();
		//}
		//else{
			//val = 1023;
			//DACR = (1<<16) | (val<<6);
		//}
	} 
}
void delay_ms(unsigned int j){
	unsigned int x,i;
	for(i=0;i<j;i++){
		for(x=0;x<10000;x++);
	}
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
void beep4(){
	short int val = 0;
			while(val!=1023){
				DACR = (1<<16) | (val<<6);
				val++;
			}
			while(val!=0){
				DACR = (1<<16) | (val<<6);
				val--;
			}
}
void beep5(){
	short int val = 0;
			while(val!=1023){
				DACR = (1<<16) | (val<<6);
				val++;
			}
}
void beep6(){
	short int val = 1023;
			DACR = (1<<16) | (val<<6);
			delay_ms(1);
			val = 0;
			DACR = (1<<16) | (val<<6);
			delay_ms(1);
}