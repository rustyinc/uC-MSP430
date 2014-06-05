/****************************************************************************
 * Project Name:    BunnyHop
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 25-03-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implements a small BunnyHop exercise. When the button is
 * pressed, a new random pattern are written to the LEDs. The LEDs that are'nt
 * illuminating, flashing one at a time.
 *
 * Port setup:
 * P2.x: LEDs
 * P1.4: Button
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/
#include <msp430.h>
#include <stdlib.h>

#define BUTTON BIT4

/* Forward Declarations */
void bunnyHop();

/*****************************************************************************
 * Main function
 *****************************************************************************/
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* Setup P2.x to output */
    P2DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2SEL  &= ~(BIT6 | BIT7); // Select the pins as GPIO
    P2SEL2 &= ~(BIT6 | BIT7);
    P2OUT = 0x00;             // Clear the outputs

    /* Setup the button as active LOW */
    P1DIR &= ~BUTTON;
    P1OUT |= BUTTON; // Set resistor to vcc
    P1REN |= BUTTON; // Enable resistor

    bunnyHop();

	return 0;
}


/*****************************************************************************
 * Bunny Hop function
 *****************************************************************************/
void bunnyHop()
{
    unsigned char bitpattern = 0;
    unsigned char bithop = 0;
    unsigned char bittest;
    for (;;) // Loop forever
    {
        if ((P1IN & BUTTON) == 0)
        {
            /* Update the bitpattern on the output*/
            bitpattern = rand() & 0xFF; // Only LSB 8 bits are used.
            P2OUT = bitpattern;
        }

        /* Check each bit position in the variable.
         * If bit is low, blink the LED
         */
        bittest = (1 << bithop); // Create variable with the bit to test
        if((bitpattern & bittest) == 0)
        {
            P2OUT ^= bittest;
            __delay_cycles(100000);
            P2OUT ^= bittest;
            __delay_cycles(100000);
        }
        else
        {
            /* LED is on - do nothing */
        }

        bithop++;
        bithop = (bithop > 7) ? 0 : bithop; // Clear bit test if > seven
    }
}
