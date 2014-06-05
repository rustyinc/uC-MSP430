#include <msp430.h> 
#include "ButtonTest.h"

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    unsigned int event;

    P2DIR = 0xFF;
    P2OUT = 0;
    setupButton(PORT1, P4, 0x1234, HIGH_TO_LOW);
    setupButton(PORT1, P5, 0x1212, HIGH_TO_LOW);
    setupButton(PORT1, P3, 0xDEAD, HIGH_TO_LOW);

    for(;;)
    {
    	event = getButtonEvent();
    	if(event == 0x1234)
    	{
    		P2OUT ^= BIT0;
    	}
    	else if(event == 0x1212)
    	{
    		P2OUT ^= BIT1;
    	}
    	else if (event == 0xDEAD)
    	{
    		P2OUT ^= BIT2;
    	}
    }

	return 0;
}
