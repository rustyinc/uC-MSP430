/****************************************************************************
 * Project Name: Lotto Number Generator
 * Author: RUST
 * Contact: rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 17.04.2014: Initial version
 *
 * =========================================================================
 * Description:
 * Binary outputs the next randomly chosen lotto number on the eight LEDs.
 * If the next lotto number is already displayed, the number is dismissed, and
 * a new randomly number is selected.
 *
 * Button one: Output the next number.
 * Button two: Resets the lotto number series
 *
 *
 * Pin setup:
 * P1.3: INPUT - BUTTON
 * P2.X: OUTPUT - LEDs
 * -------------------------------------------------------------------------
 * Results:
 * Links: -
 ****************************************************************************/
#include <msp430.h>
#include <string.h>
#include <stdlib.h>

/* Defines */
#define MAX_LOTTO_NUMBER 36
#define FALSE 0
#define TRUE !FALSE

static unsigned char lottoArray[MAX_LOTTO_NUMBER];

/***************************************************************************
 * Main function
 ***************************************************************************/
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* Port 2 setup */
    P2DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2SEL  &= ~(BIT6 | BIT7);   // Set select bits
    P2SEL2 &= ~(BIT6 | BIT7);   // Set select bits
    P2OUT = 0x00;               // Clear LEDs.

    /* Port 1 setup */
    P1DIR &= ~(BIT4 | BIT5);    // Set P1.4 and P1.5 as input
    P1IFG &= ~(BIT4 | BIT5);    // Clear interrupt flags
    P1IES |= (BIT4 | BIT5);     // Set transition high to low
    P1IE  |= (BIT4 | BIT5);     // Enable interrupts.

    /* Clear the lotto number array */
    memset(lottoArray, FALSE, MAX_LOTTO_NUMBER);

    _BIS_SR(GIE + LPM4_bits);   // Enable global interrupts + Low power mode

	return 0;
}

/***************************************************************************
 * Interrupt routine
 ***************************************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void button_isr(void)
{
    static unsigned char noLottoNumbers;
    unsigned char selected = FALSE;

    if (P1IFG & BIT4) // Button 4 pressed.
    {
        /* Pull the next lotto number */
        while(!selected)
        {
            unsigned char nextNumber = rand() % MAX_LOTTO_NUMBER;
            if (lottoArray[nextNumber] == FALSE)
            {
                selected = TRUE;                // Stop loop
                lottoArray[nextNumber] = TRUE;  // Set array as selected
                noLottoNumbers++;               // Increment no lotto numbers
                P2OUT = nextNumber + 1;         // Output number on display
            }
            else if(noLottoNumbers == MAX_LOTTO_NUMBER)
            {
                selected = TRUE;                // Stop loop
            }
            /* else - loop - pull next number */
        }
        P1IFG &= ~BIT4;                         // Clear interrupt flag
    }
    else
    {
        /* Clear lotto array */
        memset(lottoArray, FALSE, MAX_LOTTO_NUMBER);
        noLottoNumbers = 0;                     // Clear lotto number counter
        P1IFG &= ~BIT5;                         // Clear interrupt flag
        P2OUT = 0x00;                           // Clear display
    }
}
