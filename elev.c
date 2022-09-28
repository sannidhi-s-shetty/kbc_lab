#include <lpc214x.h>

unsigned int level = 0 ;
void increment_ele(void);
void delay_ms(unsigned int j);

/* int main(){
	IO0DIR |= 0xFF<<16;
	IO1DIR |= 1<<24;
	IO0CLR = 0x000F00000;
	increment_ele();
	IO0CLR=0x00F00000; IO0SET = 0x00F00000;
	delay_ms(500);
	increment_ele();
	IO0CLR=0x00F00000; IO0SET = 0x00F00000;
	delay_ms(500);
	increment_ele();
} */
void increment_ele(){
	IO0DIR |= 0xFF<<16;
	IO1DIR |= 1<<24;
	IO0CLR = 0x000F00000;
	IO0CLR=0x00F00000; IO0SET = 0x00F00000;
	level = level+1;
	int i;
	IO1CLR = 1<<24;
	IO0CLR = 0x000F00000;
		for(i=0;i<=level;i++){
			IO0CLR=0x000F0000; IO0SET=i<<16;
			delay_ms(250);
		}
}
void delay_ms(unsigned int j) 
{ 
 unsigned int x,i; 
 for(i=0;i<j;i++) 
 { 
 for(x=0; x<10000; x++);  
 } 
}
