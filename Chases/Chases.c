/****************************************************************************
 * Project Name:    Chases
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * Project Created: 21-02-14
 *
 * =========================================================================
 * Description:
 * A simple chases. The LEDs lit one by one, and starts over again when
 * the end is reached.
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/

#include <msp430.h> 

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    /* Setup port2 to output port */
    P2DIR  = 0xFF;
    P2SEL  = 0x00;
    P2SEL2 = 0x00;

    unsigned int runner = 0x01;
    unsigned int i;

    for(;;)
    {
    	runner = runner << 1;

    	/* Test if end of the chases is reached */
    	if(runner == 0x100)
    	{
    		runner = 1;
    	}
    	else
    	{
    		/* Do nothing */
    	}
    	P2OUT = runner;

    	for(i = 0; i < 6; i++)
    	{
    		P2OUT ^= runner;
    		_delay_cycles(75000);
    	}

    	_delay_cycles(50000);
    }
	return 0;
}
