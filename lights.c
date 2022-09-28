#include<lpc214x.h>
#define PLOCK 0x00000400

void PWM_Init(void);
void red(void);
void green(void);
void delay_ms(unsigned int j);
void SystemInit(void);

/*
int main()
{
SystemInit();
while(1)
{
red();
//green();
}
}
*/

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
void PWM_Init(void)
{  
    PINSEL0 |= 2 << 18 | 2 << 16 ;
   //SELECT P0.8 PWM4 AND P0.9PWM6 AS 2ND OPTION FOR PWM OPERATION
    PINSEL1 |= 1 << 10; //SELECT P0.21 PWM5 AS OPTION 1 FOR PWM OPERATION
    PWMPCR =  (1 << 12 | 1 << 13 | 1 << 14); //Enable PWM4,PWM5 and PWM        
    PWMMR0 = 100;    // load the value to MR0 to fix the pulse rate     

    PWMTCR = 0x00000009;   // bit D3 = 1 (enable  PWM), bit D0=1 (start the timer)    
}
void delay_ms(unsigned int j)
{
  unsigned int x,i;
  for(i=0;i<j;i++)
   {
      for(x=0; x<10000; x++);    
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
   PLL0FEED = 0xAA;  // lock the PLL registers after setting the required PLL
   PLL0FEED = 0x55;
   VPBDIV = 0x01;      // PCLK is same as CCLK i.e 60Mhz  
}

