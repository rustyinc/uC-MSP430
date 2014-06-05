/*
 * 24AA04_EEPROM.h
 *
 *  Created on: 10/04/2014
 *      Author: rust
 */

#ifndef 24AA04_EEPROM_H_
#define 24AA04_EEPROM_H_

unsigned int writeEEPROM(const char* data, unsigned int length);
unsigned int readEEPROM(char* data, unsigned int length);

#endif /* 24AA04_EEPROM_H_ */
