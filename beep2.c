#include<lpc214x.h>
#define PLOCK 0x00000400

void delay_ms(unsigned int j);
void SystemInit(void);

//unsigned int ans = 0; // 0 - for wrong ans, 1 - for right answer
//int main(){
//	short int i=0,val;
//	PINSEL1 = 0x00080000;
//	IO0DIR |= 1U<<31 | 0xFF<<16;  //P0.25 is output for 
//	SystemInit();
//	while(1){	
//		if(ans){  // triangular
//			beep4();
//		}
//		else {
//			beep5();
//		}
//		//else if(!SW6){
//		//	beep6();
//		//}
//		//else{
//			//val = 1023;
//			//DACR = (1<<16) | (val<<6);
//		//}
//	} 
//}


void beep4(void){
	
	PINSEL1 = 0x00080000;
	IO0DIR |= 1U<<31 | 0xFF<<16;  //P0.25 is output for 
	short int val = 1023;
			DACR = (1<<16) | (val<<6);
			delay_ms(1);
			val = 0;
			DACR = (1<<16) | (val<<6);
			delay_ms(1);
}
void beep5(void){
	
	PINSEL1 = 0x00080000;
	IO0DIR |= 1U<<31 | 0xFF<<16;  //P0.25 is output for 
	short int val = 0;
			while(val!=1023){
				DACR = (1<<16) | (val<<6);
				val++;
			}
}
void beep6(void){
	
}
