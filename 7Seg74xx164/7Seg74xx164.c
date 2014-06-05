/****************************************************************************
 * Project Name:    7Seg74xx164
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 24-03-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implements the driver for the 74xx164 bitshift register chip.
 *
 * Port setup:
 * P2.0: Data
 * P2.1: Clock
 * P2.2: Master Reset
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/

#include <msp430.h>

/* Signals for the 74xx164*/
typedef enum
{
    MR    = BIT2,
    CLOCK = BIT3,
    DATA  = BIT0
}e74xx164Signals;

typedef enum
{
    BUTTON = BIT3
}eMSP430Signals;

/* Define the different segments */
typedef enum
{
    SEG_B = BIT0,
    SEG_A = BIT1,
    SEG_F = BIT2,
    SEG_G = BIT3,
    SEG_DP = BIT4,
    SEG_C = BIT5,
    SEG_D = BIT6,
    SEG_E = BIT7
}eLEDSegments;

/* Predefined patterns  */
typedef enum
{
   LED_0 = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),         /*  0  */
   LED_1 = (SEG_B | SEG_C),                                         /*  1  */
   LED_2 = (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G),                 /*  2  */
   LED_3 = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G),                 /*  3  */
   LED_4 = (SEG_B | SEG_C | SEG_F | SEG_G),                         /*  4  */
   LED_5 = (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G),                 /*  5  */
   LED_6 = (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),         /*  6  */
   LED_7 = (SEG_A | SEG_B | SEG_C),                                 /*  7  */
   LED_8 = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), /*  8  */
   LED_9 = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G),         /*  9  */
   LED_A = (SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G),         /*  A  */
   LED_B = (SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),                 /*  b  */
   LED_C = (SEG_A | SEG_D | SEG_E | SEG_F),                         /*  C  */
   LED_D = (SEG_B | SEG_C | SEG_D | SEG_E | SEG_G),                 /*  d  */
   LED_E = (SEG_A | SEG_D | SEG_E | SEG_F | SEG_G),                 /*  E  */
   LED_F = (SEG_A | SEG_E | SEG_F | SEG_G),                         /*  F  */
   LED_H = (SEG_B | SEG_C | SEG_E | SEG_F | SEG_G),                 /*  H  */
   LED_DOT = SEG_DP,                                                /* DOT */
   LED_CLEAR = 0                                                    /*Clear*/
}eLEDPatterns;

static char patternArray[] = {
        LED_0, LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8, LED_9,
        LED_A, LED_B, LED_C, LED_D, LED_E, LED_F, LED_H, LED_DOT, LED_CLEAR
};

/**
 * Forward Declarations
 */
void writeLED(unsigned char);

/*****************************************************************************
 * Main Function
 *****************************************************************************/
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* Setup the ports for the 74HC164 chip */
    P2DIR |= (CLOCK | DATA | MR);
    /* Make sure master reset is set high */
    P2OUT |= MR;
    /* Make sure clock is initially low*/
    P2OUT &= ~CLOCK;

    /* Setup the input button P1.3 */
    P1DIR &= ~BUTTON;
    P1REN |= BUTTON;
    P1OUT |= BUTTON;

//    for(;;)
//        P2OUT ^= CLOCK;

#if 1
    unsigned int counter = 0;
    for (;;)
    {
        if ((P1IN & BUTTON) == 0)
        {
            writeLED(patternArray[counter]);

            if(++counter >= sizeof(patternArray))
            {
                counter = 0;
            }
        }
        __delay_cycles(150000);
    }
#endif
	return 0;
}

/******************************************************************************
 * Write the bitpattern
 *****************************************************************************/
void writeLED(unsigned char data)
{
    unsigned int i;
    /* Clear display */
    for(i = 0; i < 8; i++)
    {
        /* Check if bit to transmit is high or low */
        if(data & 0x01)
        {
            P2OUT |= DATA;
        }
        else
        {
            P2OUT &= ~DATA;
        }
        P2OUT |= CLOCK;
        P2OUT &= ~CLOCK;
        data >>= 1;
    }
}
