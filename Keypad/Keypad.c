/****************************************************************************
 * Project Name:    Keypad
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 18-05-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implements a keypad driver for a 4x4 keypad. When
 * the buttons are pressed, the corresponding symbols light op on the
 * 7-segment display connected to a 74xx164.
 *
 * Port setup:
 * Out : P2.[0;3] : keyboard scanning output
 * In  : P2.[4;7] : Keyboard scanning input
 *
 * In: P1.4 : Data
 * In: P1.5 : Clock
 * In: P1.6 : Master Reset
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/

#include <msp430.h>
#include "7seg74xx164Lib.h"

/**********
 * Definitions RAWs
 */
#define RAW1 BIT4
#define RAW2 BIT5
#define RAW3 BIT6
#define RAW4 BIT7
/**********
 * Definitions COLs
 */
#define COL1 BIT0
#define COL2 BIT1
#define COL3 BIT2
#define COL4 BIT3


/**********
 * Global variables
 */
static unsigned char keypressWord;

/*****************************************************************************
 * Main function
 *****************************************************************************/
void main(void)
{
    void* LEDdata;
    unsigned char lastKeyPress;

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* Initiate the timer module */
    TA0CCR0 = 0x2000;                // Set compare register to approx 20 ms
    CCTL0 = CCIE;                    // Enable timer interrupts
    TA0CTL = TASSEL_2 | ID_0 | MC_1; // SMCLK and DIVIDE BY 1 and UP

    /* Initiate port two (P2.[0;3] as output) */
    P2DIR |= (BIT0 | BIT1 | BIT2 | BIT3);
    P2OUT |= BIT0;                        // Set first bit high in output
    /* Initiate port two (P2.[4;7] as input */
    P2DIR &= ~(BIT4 | BIT5 | BIT6 | BIT7);
    P2SEL &= ~(BIT6 | BIT7);
    P2SEL2 &= ~(BIT6 | BIT7);

    P2OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7); // Set internal resistors to GND
    P2REN |= (BIT4 | BIT5 | BIT6 | BIT7);  // Enable resistors
    P2IES &= ~(BIT4 | BIT5 | BIT6 | BIT7); // Low to high transition
    P2IFG &= ~(BIT4 | BIT5 | BIT6 | BIT7); // Clear interrupt flags
    P2IE  |= (BIT4 | BIT5 | BIT6 | BIT7);  // Enable interrupts



    /* Initiate the 74xx164 driver */
    LEDdata = init7Seg74xx164Lib(PORT1, BIT4, BIT5, BIT6);

    _bis_SR_register(GIE);

    for(;;)
    {
        LPM1;

        if(lastKeyPress == keypressWord) continue;

        switch(keypressWord)
        {
            case RAW1 | COL1: writeNumber74xx164(LEDdata, 1, DOT_OFF); break;
            case RAW1 | COL2: writeNumber74xx164(LEDdata, 2, DOT_OFF); break;
            case RAW1 | COL3: writeNumber74xx164(LEDdata, 3, DOT_OFF); break;
            case RAW1 | COL4: writeNumber74xx164(LEDdata, 0x0A, DOT_OFF); break;

            case RAW2 | COL1: writeNumber74xx164(LEDdata, 4, DOT_OFF); break;
            case RAW2 | COL2: writeNumber74xx164(LEDdata, 5, DOT_OFF); break;
            case RAW2 | COL3: writeNumber74xx164(LEDdata, 6, DOT_OFF); break;
            case RAW2 | COL4: writeNumber74xx164(LEDdata, 0x0B, DOT_OFF); break;

            case RAW3 | COL1: writeNumber74xx164(LEDdata, 7, DOT_OFF); break;
            case RAW3 | COL2: writeNumber74xx164(LEDdata, 8, DOT_OFF); break;
            case RAW3 | COL3: writeNumber74xx164(LEDdata, 9, DOT_OFF); break;
            case RAW3 | COL4: writeNumber74xx164(LEDdata, 0x0C, DOT_OFF); break;

            case RAW4 | COL1: writeSelfPattern74xx164(LEDdata, (SEG_B| SEG_C | SEG_E | SEG_F | SEG_G)); break;
            case RAW4 | COL2: writeNumber74xx164(LEDdata, 0, DOT_OFF); break;
            case RAW4 | COL3: writeSelfPattern74xx164(LEDdata, (SEG_C | SEG_D | SEG_E | SEG_G)); break;
            case RAW4 | COL4: writeNumber74xx164(LEDdata, 0x0D, DOT_OFF); break;

            default:
                /* unknown combination - do nothing */
            break;
        }
        lastKeyPress = keypressWord;
    }
}

/**
 * Timer interrupt service routine
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer_interrupt(void)
{
    unsigned char keyout;

    keyout = P2OUT & (BIT0 | BIT1 | BIT2 | BIT3);
    keyout <<= 1;                           // Bitshift keyout to the right
    if (keyout > BIT3)                      // Test if keyout has to be reset
    {
        keyout = BIT0;
    }
    /* else do nothing */
    keyout |= (P2OUT & (BIT4 | BIT5 | BIT6 | BIT7));
    P2OUT = keyout;
}

/***
 * Port 2 interrupt service routine
 */
#pragma vector = PORT2_VECTOR
__interrupt void port2_interrupt(void)
{
    // Stop the timer.
    TA0CTL = TASSEL_2 | ID_1 | MC_0; // SMCLK and DIVIDE BY 4 and STOP

    keypressWord = 0;
    // Save the pressed key row.
    keypressWord |= P2OUT & (BIT0 | BIT1 | BIT2 | BIT3);
    // Save the pressed key colon
    keypressWord |= P2IN & (BIT4 | BIT5 | BIT6 | BIT7);

    // Clear the interrupt flag(s) we do only deal with one interrupt at time.
    P2IFG &= ~(BIT4 | BIT5 | BIT6 | BIT7);
    // Start the CPU again
    LPM1_EXIT;
    // Start the timer.
    TA0CTL = TASSEL_2 | ID_1 | MC_1; // SMCLK and DIVIDE BY 4 and UP
}
