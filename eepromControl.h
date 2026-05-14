#ifndef EEPROM_CONTROL_H
#define EEPROM_CONTROL_H

#include <EEPROM.h>

#define EEPROM_LOCATION 0

void eepromWrite(int address, long value);
long eepromRead(int address);
int getEepromAddress();
#endif