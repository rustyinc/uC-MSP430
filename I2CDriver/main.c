#include <msp430.h>
#include <string.h>
#include "usi_i2c.h"
#include "24AA04_EEPROM.h"


/*
 * main.c
 */

static unsigned int arrayWrite[] = {0xA0, 0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
static unsigned int arrayWriteZERO[] = {0xA0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static unsigned int arrayRead[] = {0xA0, 0, I2C_RESTART, 0xA1,
        I2C_READ, I2C_READ, I2C_READ, I2C_READ, I2C_READ,
        I2C_READ, I2C_READ, I2C_READ, I2C_READ, I2C_READ,
        I2C_READ, I2C_READ, I2C_READ, I2C_READ, I2C_READ,
        I2C_READ, I2C_READ, I2C_READ, I2C_READ, I2C_READ,
        I2C_READ, I2C_READ, I2C_READ, I2C_READ, I2C_READ};

static const char text[] = "XXXXXXXXXXXXXXXXXXXX";
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    unsigned char readin[32];
    memset(readin, 0xFF, sizeof(readin));

    /* Initiate the I2C bus */
    i2c_init(USIDIV_5, USISSEL_2);
#if 0
    writeEEPROM(text, strlen(text));
#else
    i2c_send_sequence(arrayWrite, sizeof(arrayWrite), readin, 0);
    while(!i2c_done()) ;
    __delay_cycles(25000); // Wait 25 ms
#endif

#if 1
    i2c_send_sequence(arrayRead, sizeof(arrayRead), readin, CPUOFF);
    while(!i2c_done())
        ;
#endif
    return 0;
}
