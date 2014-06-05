/****************************************************************************
 * Project Name:    KnightRiderSingleLED
 * Project Created: 21-02-14
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * -------------------------------------------------------------------------
 * Result:          www.somelink.int
 *
 * =========================================================================
 * Description:
 * A knight rider light implemented using the LED pads. The LEDs are shifted
 * back and forth as indicated in the pattern below.
 *
 * 0 - - - - - - -
 * - 0 - - - - - -
 * - - 0 - - - - -
 * - - - 0 - - - -
 * - - - - 0 - - -
 * - - - - - 0 - -
 * - - - - - - 0 -
 * - - - - - - - 0
 * - - - - - - 0 -
 * - - - - - 0 - -
 * - - - - 0 - - -
 * - - - 0 - - - -
 * - - 0 - - - - -
 * - 0 - - - - - -
 * 0 - - - - - - -
 *
 * Note that this implementation is quite advanced, due to the usage of
 * function pointers.
 ****************************************************************************/
#include <msp430.h> 

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    /* Set all pins on P2.x to output */
    P2DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
    /* Make sure the upper two "select bits" are low, to enable BIT6 and BIT7 */
    P2SEL  &= ~(BIT6 | BIT7);
    P2SEL2 &= ~(BIT6 | BIT7);

    /**
     * Instantiate direction counter:
     * if counter is 0 direction is up
     * if counter is 1 direction is down
     */
    unsigned char direction = 0;
    /**
     * Write initial value to output port
     */
    P2OUT = 0x01;

    for(;;)
    {
    	if(direction == 0)
    	{
    		P2OUT = P2OUT <<= 1;
    		direction = (P2OUT == 0x80) ? 1 : 0;
    	}
    	else
    	{
    		P2OUT = P2OUT >>= 1;
    		direction = (P2OUT == 0x01) ? 0 : 1;
    	}
    	_delay_cycles(50000);
    }
	return 0;
}
