/****************************************************************************
 * Project Name:    FibonacciCounter
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 25-03-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implement a binary fibonacci counter. The LEDs are updated to
 * reflect the next fibonacci value, when the button is pressed.
 *
 * Port setup:
 * P2.x: LED Output
 * P1.3: Button Input
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/
#include <msp430.h>

#define BUTTON BIT4

/* Forward declaration*/
void fibonacciCount();
void blinkLEDs();

/****************************************************************************
 * Main function
 ****************************************************************************/
int main(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    /* Setup port 2 to output */
    P2DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    /* Select P2.6 and P2.7 */
    P2SEL  &= ~(BIT6 | BIT7);
    P2SEL2 &= ~(BIT6 | BIT7);
    /* Clear the LEDs */
    P2OUT = 0x00;

    /* Setup P1.3 as input */
    P1DIR &= ~BUTTON;
    /* Setup and enable resistor (set high) */
    P1OUT |= BUTTON;
    P1REN |= BUTTON;

    fibonacciCount();

	return 0;
}

/*****************************************************************************
 * Fibonacci function
 *****************************************************************************/
void fibonacciCount()
{
    unsigned int tmpLast;
    unsigned int last = 1;
    unsigned int current = 0;

    for(;;) /* Loop forever */
    {
        /* Check for button press*/
        if ((P1IN & BUTTON) == 0)
        {
            /* (0) 1 1 2 3 5 8 13 21 34 55 89 144 233 */
            tmpLast = last;
            last = current;
            current = current + tmpLast;
            if(current > 0xFF)
            {
                /* Blink the LEDs - reset counter */
                blinkLEDs();
                last = 1;
                current = 0;
            }
            else
            {
                /* Write out the next fibonacci number */
                P2OUT = current;
            }
        }
        else
        {
            /* Button not pressed - do nothing */
        }
        __delay_cycles(150000);
    }
}

/*****************************************************************************
 * blinkLEDs
 *****************************************************************************/
void blinkLEDs()
{
    unsigned int cnt;

    P2OUT = 0x0; /* Initially turn off all LEDs */
    for(cnt = 0; cnt < 6; cnt++)
    {
         /* Toggle (reverse) all LEDs */
        P2OUT = ~P2OUT;
        __delay_cycles(75000);
    }
}
