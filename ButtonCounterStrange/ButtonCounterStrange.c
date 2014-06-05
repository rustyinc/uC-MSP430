/****************************************************************************
 * Project Name:    Button Counter Int
 * Project Created: 13-02-14
 * Author:          RUST
 *
 * ------------------------------------------------------------------------
 * Version history:
 * 13-02-14: Initial version
 *
 * =========================================================================
 * Description:
 * The LEDs are updated by using bitshifting.
 * The button press is implemented using interrupts.
 *
 ****************************************************************************/

#include <msp430.h> 

static unsigned int counter = 0; // Set the counter

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /**
     * Remember:
     * & -> is an and-operation (0 & 0 = 0, 0 & 1 = 0, 1 & 0 = 0, 1 & 1 = 1)
     * | -> is an or-operation  (0 | 0 = 0, 0 | 1 = 1, 1 | 0 = 1, 1 & 1 = 1)
     */
    P1DIR = P1DIR & 0xF7; // Setup port P1.3 to input. (clear bit 4)
    P1REN = P1REN | 0x08; // Set internal resistor to on for port P1.3.
    P1OUT = P1OUT | 0x08; // Pull up resistor - Button is active LOW.
    P1IES = P1IES | 0x08; // Interrupt edge selection - High-To-Low.
    P1IFG = P1IFG & 0xF7; // Initially make sure the interrupt flag is cleared
    P1IE  = P1IE  | 0x08; // Enable interrupt on P1.3

    /* Setup port to as output */
    P2DIR  = 0xFF;         // Set all pins in port 2 to output
    P2SEL  = 0;            // Necessary in order to enable Port 2.6 and port 2.7
    P2SEL2 = 0;            // --------------------- " --------------------------

    P2OUT  = counter;      // Set LEDs to initial value

    _BIS_SR(GIE);    // Global interrupt enable

    return 0;
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	// TODO: What happens here?? Try to explain (Use the Debugger - look at the counter variable)
	if(counter > 0 && counter < 0xFF)
	{
		counter = counter << 1 | counter;
	}
	else if(counter == 0xFF)
	{
		counter = 0;
	}
	else
	{
		counter = 1;
	}

	P2OUT = counter;      // Write counter to LEDs
	P1IFG = P2IFG & 0xF7; // Clear interrupt

}
