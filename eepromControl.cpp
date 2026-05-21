/**
 * @file eepromControl.cpp
 * @brief Implementation of EEPROM counter management functions
 */

#include "eepromControl.h"

/**
 * @brief Increments and saves counter to EEPROM
 *
 * Reads current count from EEPROM, increments it by 1,
 * and writes back using EEPROM.update() which only writes
 * if the value has changed, reducing wear.
 *
 * Counter stored as 16-bit little-endian (LSB first).
 */
void updateCountInEeprom()
{
    uint16_t address = getEepromLocationAddress();
    uint16_t count = getTotalCountFromEeprom();
    
    // Increment the count
    count++;
    
    // Write both bytes using EEPROM.update() which only writes
    // if value changed. This reduces wear and improves reliability
    EEPROM.update(address, (count & 0xFF));
    EEPROM.update(address + 1, ((count >> 8) & 0xFF));
}

/**
 * @brief Reads counter value from current EEPROM location
 *
 * Reads 16-bit counter stored in little-endian format.
 * Validates address bounds before reading.
 *
 * @return uint16_t Current count value, or 0xFFFF if address
 *         is out of bounds (invalid marker)
 */
uint16_t getTotalCountFromEeprom()
{
    uint16_t data = 0;
    uint16_t address = getEepromLocationAddress();
    
    // Validate address is within bounds
    if(address + 1 >= EEPROM.length())
    {
        return 0xFFFF; // Return invalid marker
    }
    
    // Read 16-bit value (little-endian)
    data = EEPROM.read(address);
    data |= ((uint16_t)EEPROM.read(address + 1)) << 8;
    return data;
}

/**
 * @brief Clears counter at current EEPROM location
 *
 * Writes zero to both bytes of counter storage.
 * Validates address bounds before writing.
 */
void clearEepromCounter()
{
    uint16_t address = getEepromLocationAddress();
    
    // Validate address is within bounds before clearing
    if(address + 1 < EEPROM.length())
    {
        EEPROM.update(address, 0);
        EEPROM.update(address + 1, 0);
    }
}

/**
 * @brief Gets current EEPROM storage location address
 *
 * Reads 16-bit location pointer from addresses 0-1.
 * Validates pointer and auto-corrects if corrupted.
 *
 * Validation rules:
 * - Must be >= 4 (addresses 0-3 reserved)
 * - Must be even (2-byte alignment)
 * - Must be within EEPROM bounds
 * - Must not be 0xFFFF (uninitialized)
 *
 * @return uint16_t Valid EEPROM address (4-1022, even only)
 */
uint16_t getEepromLocationAddress()
{
    uint16_t address = 0;
    
    // Read 16-bit location pointer from addresses 0-1
    address = EEPROM.read(EEPROM_LOCATION_ADDR_LSB);
    address |= ((uint16_t)EEPROM.read(EEPROM_LOCATION_ADDR_MSB)) << 8;
    
    // Validate and fix corrupted location pointer
    // Location must be >= 4 (avoid overwriting location ptr at 0-1
    // and maintain 2-byte alignment, skip addresses 2-3)
    // Location must be even (for 2-byte alignment)
    // Location must be within EEPROM bounds
    if(address == 0xFFFF   ||
       address < 4         ||
       (address % 2) != 0  ||
       address + 1 >= EEPROM.length())
    {
        // Reset to initial valid location (address 4)
        address = 4;
        EEPROM.update(EEPROM_LOCATION_ADDR_LSB, address & 0xFF);
        EEPROM.update(EEPROM_LOCATION_ADDR_MSB, (address >> 8) & 0xFF);
    }
    
    return address;
}

/**
 * @brief Moves to next EEPROM location for wear leveling
 *
 * Increments location pointer by 2 bytes (size of counter).
 * Wraps around to address 4 if end of EEPROM is reached.
 * This distributes write cycles across EEPROM memory.
 *
 * @return uint16_t New location address where next counter
 *         will be stored
 */
uint16_t updateEepromAddressForNextCycle()
{
    uint16_t location = getEepromLocationAddress();

    // Calculate next location (current + 2 bytes for counter)
    uint16_t nextLocation = location + 2;
    
    // Check if next location exceeds EEPROM size
    // Ensure nextLocation + 1 is valid (for 2-byte storage)
    if(nextLocation + 1 >= EEPROM.length())
    {
        // Wrap to beginning (address 4, avoiding 0-3)
        nextLocation = 4;
    }
    
    // Double-check the location is valid before writing
    if(nextLocation < 4 || (nextLocation % 2) != 0)
    {
        nextLocation = 4;
    }
    
    // Update location pointer (16-bit little-endian)
    EEPROM.update(EEPROM_LOCATION_ADDR_LSB, nextLocation & 0xFF);
    EEPROM.update(EEPROM_LOCATION_ADDR_MSB, (nextLocation >> 8) & 0xFF);

    return nextLocation;
}