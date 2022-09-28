#include <LPC214x.h> 
#define SW2 (IO0PIN & (1 << 14))
void delayHW(void) {
	T0PR = 60000-1; // produces 1msec delay 
	T0MR0 = 50; // produces 5000 x 1msec delay = 5seconds 
	T0TCR = 1; // Start the timer 
	while (!(T0TC == T0MR0)); // wait for TC to reach MR0 
	T0TCR = 2;
// Stop the timer, reset the TC,PC 
}
int main(void) {
	IODIR0 = (1<<11);
	while(1) {
	if(1) // wait for key press, if yes produces 5 sec pulse 
		{
			IOSET0 = 1 << 10; //set P0.10 to 1 
			delayHW( ); 
			IOCLR0 = 1 <<10; //clear P0.10 to 1
		} 
	} 
}
