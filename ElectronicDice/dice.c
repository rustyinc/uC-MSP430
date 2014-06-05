/****************************************************************************
 * Project Name:    Electronic Dice
 * Project Created: 19-02-14
 * Author:          RUST
 *
 * ------------------------------------------------------------------------
 * Version history:
 * 19-02-14: Initial version
 *
 * =========================================================================
 * Description:
 * An electronic dice . When button P1.4 is pressed, the
 * LEDs on the LED pad changes shape:
 *
 * LED is as follows: (LED 8 always off)
 * 1: OFF  OFF  OFF  ON   OFF  OFF  OFF
 * 2: OFF  ON   OFF  OFF  OFF  ON   OFF
 * 3: OFF  ON   OFF  ON   OFF  ON   OFF
 * 4: ON   OFF  ON   OFF  ON   OFF  ON
 * 5: ON   OFF  ON   ON   ON   OFF  ON
 * 6: ON   ON   ON   OFF  ON   ON   ON
 *
 ****************************************************************************/
#include <msp430.h>

#define ONE   (BIT3)
#define TWO   (BIT1 | BIT5)
#define THREE (ONE  | TWO)
#define FOUR  (BIT0 | BIT2 | BIT4 | BIT6)
#define FIVE  (FOUR | ONE)
#define SIX   (FOUR | TWO)

static const char diceAry[] = {
        ONE, TWO, THREE, FOUR, FIVE, SIX
};

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    /* Setup input on button P1.4 */
    P1DIR &= ~BIT4;

    /* Setup Output on port P2.0*/
    P2DIR |= 0xFF;
    P2SEL  = 0x00;
    P2SEL2 = 0x00;

    unsigned int count = 0;

    for(;;)
    {
        if((P1IN & BIT4) == 0)
        {
            count++;
            count = (count >= sizeof(diceAry)) ? 0 : count;
            _delay_cycles(12000);
        }
        P2OUT = diceAry[count];
    }

    return 0;
}
