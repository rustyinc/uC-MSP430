/****************************************************************************
 * Project Name:    Binary Clock
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 21-02-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implements a binary Clock, by using the LED pad.
 *
 * Port setup:
 * The LED on  P2.7 flashes every second to indicated seconds.
 * The LED on  P2.6 ON:  P2.0 - P2.5 shows minutes.
 *             P2.6 OFF; P2.0 - P2.3 shows hours
 * The LEDs on P2.0 to P2.5 indicate minutes. (0-59)
 * The LEDs on P2.0 and P2.3 indicate hours
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: http://www.youtube.com/watch?v=3iEJtNcCw_k
 ****************************************************************************/
#include <msp430.h>

typedef enum {
	DISPLAY_HOURS,
	DISPLAY_MINUTES
}eDisplay;

#define TURN_ON_SEC(x) ((x) |= (BIT7))
#define TURN_OFF_SEC(x) ((x) &= (~BIT7))

typedef struct {
	eDisplay display;
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
}clock_t;

/* Forward declerations */
static void Clock();
static void UpdateTime(clock_t*);
static void UpdateDisplay(clock_t*);

/**
 * The entrypoint for the Binary watch program
 */
int main(void) {
	/* Stop watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Setup P2.x as output ports */
    P2DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
	/* Clear select bits, to enable LEDs on P2.6 and P2.7 */
    P2SEL  &= ~(BIT6 | BIT7);
    P2SEL2 &= ~(BIT6 | BIT7);

    /* Make sure output ports are turned off. */
    P2OUT &= 0x00;

    Clock();

	return 0;
}

/**
 * Watch: Function that handles the watch functionality
 * Displays shift between minutes and hours every 4 seconds.
 * It is assumed, that 1.000.000 cyckes ~ 1 second
 */
void Clock()
{
	/* Initiate variables for seconds, minutes and hours. */
	clock_t clock;
	clock.hours = 23;
	clock.minutes = 59;
	clock.seconds = 00;
	clock.display = DISPLAY_HOURS;

	for(;;)
	{
		/* Update and blink second */
		TURN_ON_SEC(P2OUT);
		UpdateTime(&clock);
		UpdateDisplay(&clock);
		_delay_cycles(50000);
		TURN_OFF_SEC(P2OUT);

		_delay_cycles(950000);
	}
}

/**
 * Update the hours, minutes and seconds
 */
void UpdateTime(clock_t* clock)
{
	clock->seconds++;
	if(clock->seconds == 60)
	{
		/* Seconds is 60 - reset to zero */
		clock->seconds = 0;
		/* Update minutes*/
		clock->minutes++;
		if(clock->minutes == 60)
		{
			/* Minutes is 60 - reset to zero */
			clock->minutes = 0;
			/* Update hours */
			clock->hours++;
			if(clock->hours == 24)
			{
				/* Hours is 12 - reset to zero again*/
				clock->hours = 0;
			} /* else - do nothing */
		} /* else - do nothing */
	} /* else - do nothing */
}

/**
 * Update LED display
 */
void UpdateDisplay(clock_t* clock)
{
	unsigned char time;
	/* Update the display state (every four seconds, change states) */
	if((clock->seconds & 0x03) == 0)
	{
		if(clock->display == DISPLAY_HOURS)
		{
			clock->display = DISPLAY_MINUTES;
		}
		else
		{
			clock->display = DISPLAY_HOURS;
		}
	}

	/* Extract display */
	time = P2OUT;
	/* Clear the lowest 7 bits */
	time &= 0x80;

	/* Show clock */
	if(clock->display == DISPLAY_HOURS)
	{
		/* Update the display */
		time |= clock->hours & 0x3F;
		P2OUT = time;
	}
	else
	{
		/* Show minutes, and the minute LED */
		time |= (clock->minutes & 0x3F) | BIT6;
		P2OUT = time;
	}
}
