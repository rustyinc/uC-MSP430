#include <msp430.h>

/*
 * main.c
 */
#define DOT BIT6
#define DOT_ON(x) ((x) &= (~DOT))
#define DOT_OFF(x) (((x) |= (DOT))
#define DOT_TOGGLE(x) ((x) ^= (DOT))

static const unsigned char SevenSegAry[] = {
		~(BIT0 | BIT1 | BIT3 | BIT4 | BIT5 | BIT7),        /* 0 */
		~(BIT5 | BIT7),                                    /* 1 */
		~(BIT0 | BIT2 | BIT3 | BIT4 | BIT7),               /* 2 */
		~(BIT0 | BIT2 | BIT4 | BIT5 | BIT7),               /* 3 */
		~(BIT1 | BIT2 | BIT5 | BIT7),                      /* 4 */
		~(BIT0 | BIT1 | BIT2 | BIT4 | BIT5),               /* 5 */
		~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5),        /* 6 */
		~(BIT0 | BIT5 | BIT7),                             /* 7 */
		~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7), /* 8 */
		~(BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT7),        /* 9 */
		~(BIT0 | BIT1 | BIT2 | BIT3 | BIT5 | BIT7),        /* A */
		~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5),               /* B */
        ~(BIT0 | BIT1 | BIT3 | BIT4),                      /* C */
        ~(BIT2 | BIT3 | BIT4 | BIT5 | BIT7),               /* D */
        ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4),               /* E */
        ~(BIT0 | BIT1 | BIT2 | BIT3),                      /* F */
};

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P2DIR = 0xFF;
    P2SEL = 0x00;
    P2SEL2 = 0x00;

    P1DIR &= ~BIT3;

    unsigned int count = 0;
    for(;;)
    {
    	if((P1IN & BIT3) > 0)
    	{
    		P2OUT = SevenSegAry[count++];
    		count = (count == sizeof(SevenSegAry) ? 0 : count);
    	}
        _delay_cycles(20000);
    }
	return 0;
}
