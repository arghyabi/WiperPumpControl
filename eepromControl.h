/**
 * @file eepromControl.h
 * @brief EEPROM management for persistent counter storage
 *
 * Implements wear-leveling by rotating storage locations across
 * EEPROM memory. Location pointer stored at addresses 0-1.
 */

#ifndef EEPROM_CONTROL_H
#define EEPROM_CONTROL_H

#include <EEPROM.h>

// Location pointer uses 2 bytes (addresses 0-1)
#define EEPROM_LOCATION_ADDR_LSB 0
#define EEPROM_LOCATION_ADDR_MSB 1

/**
 * @brief Clears the counter at current EEPROM location
 *
 * Writes zero to both bytes of the counter storage.
 * Validates address bounds before writing.
 */
void clearEepromCounter();

/**
 * @brief Increments and saves counter to EEPROM
 *
 * Reads current count, increments by 1, and writes back
 * to EEPROM using update() to minimize wear.
 */
void updateCountInEeprom();

/**
 * @brief Reads counter value from current EEPROM location
 *
 * @return uint16_t Current count (0-2000), or 0xFFFF if invalid
 */
uint16_t getTotalCountFromEeprom();

/**
 * @brief Gets current EEPROM storage location address
 *
 * Reads 16-bit location pointer from addresses 0-1.
 * Validates and auto-corrects if corrupted.
 *
 * @return uint16_t Valid EEPROM address (4-1022, even only)
 */
uint16_t getEepromLocationAddress();

/**
 * @brief Moves to next EEPROM location for wear leveling
 *
 * Increments location by 2 bytes. Wraps to address 4
 * if end of EEPROM reached.
 *
 * @return uint16_t New location address
 */
uint16_t updateEepromAddressForNextCycle();

#endif
