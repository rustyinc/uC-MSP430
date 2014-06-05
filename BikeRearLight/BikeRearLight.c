/****************************************************************************
 * Project Name:    Bike Rear Light
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 09-03-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implements a Bike Rear Light using the LED pad.
 *
 * When the button is pressed, the LED Patterns change.
 *
 * S_OFF ---> S_SOLID ---> S_BLINK ---> S_CHASES
 *   ^                                      |
 *   |______________________________________|
 *
 * Port setup:
 * P2.x: LED outputs
 * P1.4: Button Input
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/

#include <msp430.h>

/* States */
typedef enum
{
    S_OFF, S_SOLID, S_BLINK, S_CHASES
}sRearLights;

/* Forward declarations */
void BikeRearLight();

/****************************************************************************
 * Main function:
 ****************************************************************************/
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* Port 2 setup as LED output */
    P2DIR = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
    P2SEL  &= ~(BIT6 | BIT7);
	P2SEL2 &= ~(BIT6 | BIT7);

	/* Port 1.4 setup as input */
	P1DIR &= ~BIT4;

	BikeRearLight();

    return 0;
}

/****************************************************************************
 *
 ****************************************************************************/
void BikeRearLight()
{
    unsigned char chase = 0x01; /* Initial chase LED pattern */
    sRearLights lightState = S_OFF; /* initial state */

    for(;;)
    {
        if((P1IN & BIT4) == 0x00)
        {
            /* Button is pressed - change pattern */
            switch(lightState)
            {
                case S_OFF:    lightState = S_SOLID;  break;
                case S_SOLID:  lightState = S_BLINK;  break;
                case S_BLINK:  lightState = S_CHASES; break;
                case S_CHASES: lightState = S_OFF;    break;
                default:       lightState = S_OFF;    break;
            }
        }
        else
        {
            /* Do nothing */
        }

        switch(lightState)
        {
            case S_OFF:
                /* Turn off All LEDs */
                P2OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
            break;

            case S_SOLID:
                /* Turn on All LEDs */
                P2OUT |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
            break;

            case S_CHASES:
                P2OUT = chase;
                chase <<= 1;
                chase = (chase == 0x00) ? 0x01 : chase;
            break;

            case S_BLINK:
                P2OUT ^= 0xFF;
            break;
        }
        /* Wait approx 100 ms.*/
        __delay_cycles(150000);
    }
}
