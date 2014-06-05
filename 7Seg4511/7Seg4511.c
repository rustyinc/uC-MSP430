#include <msp430.h>

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    P2DIR = 0xFF;

    P2SEL = 0x00;
    P2SEL2 = 0x00;

    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;

//    0 0 1 1 0 0 0 0

    P2OUT = 0x30;
    unsigned char counter = 0x00;
    for(;;)
    {
        P2OUT &= 0xF0;
        P2OUT |= (counter & 0x0F);

        if(++counter >= 10) counter = 0;
        __delay_cycles(500000);

        if((P1IN & BIT3) == 0)
        {
            P2OUT ^= 0x80;
        }
    }


    return 0;
}
