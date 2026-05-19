#include "eepromControl.h"

void updateCountInEeprom()
{
    uint8_t address = getEepromLocationAddress();
    uint16_t count = getTotalCountFromEeprom();
    
    // update the count
    count++;
    
    EEPROM.write(address, (count & 0xFF));
    EEPROM.write(address + 1, ((count >> 8) & 0xFF));
}

uint16_t getTotalCountFromEeprom()
{
    uint16_t data = 0;
    uint8_t address = getEepromLocationAddress();
    data = EEPROM.read(address);
    data = ((EEPROM.read(address + 1)) << 8) | data;
    return data;
}


uint8_t getEepromLocationAddress()
{
    uint8_t address = (uint8_t)EEPROM.read(EEPROM_LOCATION_ADDR);
    return address;
}

// it will return the address of the next location to be written
void updateEepromAddressForNextCycle()
{
    uint8_t location = getEepromLocationAddress();

    // If the location address is uninitialized or has already reached 255 after increment,
    // then it will be initialised with 2.
    if(location == 0xFF) 
    {
        EEPROM.write(EEPROM_LOCATION_ADDR, 2);
    }

    // if the value is odd, make it even; it should not come here, but this is for precaution.
    if(location % 2)
    {
        location = location & 0xFE;
    }

    // To save value 0->2000, a total of 2 bytes of location is needed. 
    // So it update the exsiting location + 2.
    EEPROM.write(EEPROM_LOCATION_ADDR, (location + 2));
}