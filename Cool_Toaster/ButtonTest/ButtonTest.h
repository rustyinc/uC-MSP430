/*
 * ButtonTest.h
 *
 *  Created on: 03/02/2014
 *      Author: rust
 */

#ifndef BUTTONTEST_H_
#define BUTTONTEST_H_

typedef enum {
	LOW_TO_HIGH,
	HIGH_TO_LOW
}edgeDirection;

typedef enum {
	PORT1,
	PORT2
}portSelection;

typedef enum {
	P0 = BIT0,
	P1 = BIT1,
	P2 = BIT2,
	P3 = BIT3,
	P4 = BIT4,
	P5 = BIT5,
	P6 = BIT6,
	P7 = BIT7
}pinSelection;

/* Function to setup input on a port */
void setupButton(portSelection port, pinSelection pin, int event, edgeDirection dir);

/* Returns an button event when a button is pressed */
int getButtonEvent();

#endif /* BUTTONTEST_H_ */
