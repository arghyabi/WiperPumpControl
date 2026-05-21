#ifndef EEPROM_CONTROL_H
#define EEPROM_CONTROL_H

#include <EEPROM.h>

#define EEPROM_LOCATION_ADDR 0

void     clearEepromCounter();
void     updateCountInEeprom();
uint16_t getTotalCountFromEeprom();
uint8_t  getEepromLocationAddress();
uint8_t  updateEepromAddressForNextCycle();

#endif
