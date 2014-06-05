/****************************************************************************
 * Project Name:    KnightRider Light
 * Project Created: 19-02-14
 * Author:          RUST
 *
 * ------------------------------------------------------------------------
 * Version history:
 * 19-02-14: Initial version
 *
 * =========================================================================
 * Description:
 * A knight rider light implemented using the LED pads. The LEDs are shifted
 * back and forth as indicated in the pattern below.
 *
 * 0 0 - - - - - -
 * - 0 0 - - - - -
 * - - 0 0 - - - -
 * - - - 0 0 - - -
 * - - - - 0 0 - -
 * - - - - - 0 0 -
 * - - - - - - 0 0
 * - - - - - 0 0 -
 * - - - - 0 0 - -
 * - - - 0 0 - - -
 * - - 0 0 - - - -
 * - 0 0 - - - - -
 * 0 0 - - - - - -
 *
 * Note that this implementation is quite advanced, due to the usage of
 * function pointers.
 ****************************************************************************/

#include <msp430.h> 

int dirLeft(int outval)
{
	return outval << 1;
}

int dirRight(int outval)
{
	return outval >> 1;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    /* Setup output port on P2.x */
    P2DIR  = 0xFF;
    P2SEL  = 0x00;
    P2SEL2 = 0x00;

    /* Initial P2.x output value */
    unsigned char outval = 0x03;
    int (*dir)(int)  = dirLeft;

    for(;;)
    {
    	/* Print the output value */
    	P2OUT = outval;

    	/* Calculate the next value */
    	if(outval == 0x03)
    	{
    		/* Light has reached lowest end */
    		dir = dirLeft;
    	}
    	else if(outval == 0xC0)
    	{
    		/* Light has reached highest end */
    		dir = dirRight;
    	}
    	else
    	{
    		/* Do absolutely nothing */
    	}
    	outval = dir(outval);
    	_delay_cycles(50000);
    }

	return 0;
}
