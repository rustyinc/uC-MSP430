#include <msp430.h>

typedef enum
{
    BUTTON_3,
    BUTTON_4
}eButtonEvents;

eButtonEvents buttonEvent;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    // setup the output:
    P2DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    P2SEL  &= ~(BIT7 | BIT6);
    P2SEL2 &= ~(BIT7 | BIT6);

    // Setup input P1.4
    P1DIR &= ~(BIT4);
    P1IES |= BIT4; // High to low transistion
    P1IFG &= ~(BIT4);
    P1IE  |= BIT4;

    // Setup input P1.3
    P1DIR &= ~(BIT3);
    P1OUT |= BIT3; // Used to set resistor to VCC
    P1REN |= BIT3; // Enable resistor
    P1IES |= BIT3; // High to low transistion
    P1IFG &= ~(BIT3);
    P1IE  |= BIT3;


    _BIS_SR(GIE); /* Enable interrupts globally */

    P2OUT = 0x00; // Clear the port
    for(;;)
    {
        LPM4;
        if(buttonEvent == BUTTON_4)
        {
            P2OUT += 1;
        }
        else
        {
            P2OUT -= 1;
        }
    }

}

#pragma vector = PORT1_VECTOR
__interrupt void buttonISR(void)
{
    unsigned char bit;
    if(P1IFG & BIT3)
    {
        buttonEvent = BUTTON_3;
        bit = BIT3;
    }
    else
    {
        buttonEvent = BUTTON_4;
        bit = BIT4;
    }
    P1IFG &= ~(bit);
    LPM4_EXIT;
}
