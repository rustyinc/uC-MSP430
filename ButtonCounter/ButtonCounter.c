/****************************************************************************
 * Project Name:    Button Counter
 * Project Created: 13-02-14
 * Author:          RUST
 *
 * ------------------------------------------------------------------------
 * Version history:
 * 13-02-14: Initial version
 *
 * =========================================================================
 * Description:
 * Every time the button is pressed the LEDs is updated with a new binary
 * value. The button press is implemented using polling.
 *
 ****************************************************************************/

#include <msp430.h> 

int main(void) {
	int counter = 0; // Set the counter variable initially to zero
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /**
     * Remember:
     * & -> is an and-operation (0 & 0 = 0, 0 & 1 = 0, 1 & 0 = 0, 1 & 1 = 1)
     * | -> is an or-operation  (0 | 0 = 0, 0 | 1 = 1, 1 | 0 = 1, 1 & 1 = 1)
     */
    P1DIR = P1DIR & 0xF7; // Setup port P1.3 to input. (clear bit 4)
    P1REN = P1REN | 0x08; // Set internal resistor to on for port P1.3.
    P1OUT = P1OUT | 0x08; // Pull up resistor - Button is active LOW.

    /* Setup port to as output */
    P2DIR  = 0xFF;         // Set all pins in port 2 to output
    P2SEL  = 0;            // Necessary in order to enable Port 2.6 and port 2.7
    P2SEL2 = 0;            // --------------------- " --------------------------

    P2OUT  = 0;            // Turn off all the LEDs

    while(1) // Loop forever
    {
    	if((P1IN & 0x08) == 0) // Test if P1.3 is LOW (Button is pressed)
    	{
    		counter = counter + 1; // Increment counter by one
    		/* Above incrementation can also be written. counter += 1 or simply counter++ */

    		/* Write the new counter value to the port */
    		P2OUT = counter;
    	}
    	else
    	{
    		/* Button is not pressed - do nothing*/
    	}
    }
	return 0;
}
