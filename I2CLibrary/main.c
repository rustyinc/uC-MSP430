#include <msp430.h>
#include "i2c.h"

/*
 * main.c
 */
int main(void)
{
    volatile unsigned char ret = 0xFF;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    i2c_init();
//    i2c_start();
//    ret = i2c_write8(0xA0);
//    ret = i2c_write8(0x01);
//    ret = i2c_write8(0x55);
//    i2c_stop();

    i2c_start();
    ret = i2c_write8(0xA0);
    ret = i2c_write8(0x00);
    i2c_stop();
    i2c_start();
    ret = i2c_write8(0xA1);
    ret = i2c_read8(0x00);
    ret = i2c_read8(0x00);
    ret = i2c_read8(0x00);
    ret = i2c_read8(0x00);
    ret = i2c_read8(0x00);
    ret = i2c_read8(0x00);
    ret = i2c_read8(0x00);
    ret = i2c_read8(0xFF);
    i2c_stop();

	return 0;
}
