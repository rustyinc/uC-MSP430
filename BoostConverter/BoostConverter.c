#include <msp430.h>
#include "lcd16.h"

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P1DIR |= BIT0; // Setup P1.0 to output
    P1OUT |= BIT0; // Set P1.0 high

    P2DIR  = 0xFF;
    P2SEL  = 0x00;
    P2SEL2 = 0x00;

    TA0CCR0 = 500;
    CCTL0 = CCIE;
    TA0CTL = TASSEL_2 | ID_0 | MC_1; // SMCLK and DIVIDE BY 1 and UP

    _BIS_SR(GIE); // Enter LPM4 w/ Global interrupt enable

    __delay_cycles(1000000);
    /* Enable display */

    lcdinit();
    prints("TEST");
    while(1);
	return 0;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_interrupt(void)
{
	P1OUT ^= BIT0;
}
