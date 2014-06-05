/****************************************************************************
 * Project Name:    ClockCrystal
 * File:            clockCrystal.c
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 02-04-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * Testing the clock crystal on the input
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/
#include <msp430.h>

#if 1
unsigned char count = 0;
int main(void)
{
    /* Stop watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Set LFXTCLK to low frequency */
//    BCSCTL1 &= ~XTS;
    /* Set SUB MASTER Clock to LFXT1CLK */
//    BCSCTL2 |= SELS;
    /* Select Clock crystal */
//    BCSCTL3 &= ~(LFXT1S0 | LFXT1S1);
    /* Set to 10 pF*/
    BCSCTL3 |= XCAP_1;

    P1DIR |= BIT6;

    TA0CCTL0 = CCIE;
    TA0CCR0 = 0xFFFF;
    TA0CTL = TASSEL_2 + ID_0 + MC_1; // ACLK and DIVIDE BY 1 and UP


    _BIS_SR(LPM0_bits + GIE); // Enter LPM4 w/ Global interrupt enable


    return 0;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_interrupt(void)
{
    P1OUT ^= BIT6;
}
#else

unsigned int currentMinutes, currentSeconds;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop WDT

    BCSCTL1 |= DIVA_3;              // ACLK/8
    BCSCTL3 |= XCAP_3;              //12.5pF cap- setting for 32768Hz crystal

    P1DIR |= BIT0;                  // set P1.0 (LED1) as output
    P1OUT |= BIT0;                  // P1.0 low

    currentMinutes = 0;
    currentSeconds = 0;

    CCTL0 = CCIE;                   // CCR0 interrupt enabled
    CCR0 = 511;                 // 512 -> 1 sec, 30720 -> 1 min
    TACTL = TASSEL_1 + ID_3 + MC_1;         // ACLK, /8, upmode

    _BIS_SR(LPM3_bits + GIE);           // Enter LPM3 w/ interrupt
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    P1OUT ^= BIT0;                  // Toggle LED
}
#endif
