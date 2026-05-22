/**
 * @file lcdControl.h
 * @brief LCD display control for 20x4 I2C LCD module
 */

#ifndef LCD_CONTROL_H
#define LCD_CONTROL_H

#include <LiquidCrystal_I2C.h>

#define LCD_MODULE_NO_OF_ROW        4
#define LCD_MODULE_NO_OF_COLUMN     20

/**
 * @brief Initialize LCD display
 * @param lcd LCD object reference
 */
void initLCD(LiquidCrystal_I2C &lcd);

/**
 * @brief Display system startup message
 * @param lcd LCD object reference
 */
void displayStartup(LiquidCrystal_I2C &lcd);

/**
 * @brief Display current cycle count and remaining
 * @param lcd LCD object reference
 * @param current Current cycle number
 * @param total Total cycles to complete
 */
void displayCount(LiquidCrystal_I2C &lcd, uint16_t current,
                  uint16_t total);

/**
 * @brief Display motor status
 * @param lcd LCD object reference
 * @param isOn true if motor is ON, false if OFF
 */
void displayMotorStatus(LiquidCrystal_I2C &lcd, bool isOn);

/**
 * @brief Display completion message
 * @param lcd LCD object reference
 */
void displayComplete(LiquidCrystal_I2C &lcd);

/**
 * @brief Display error message
 * @param lcd LCD object reference
 * @param errorMsg Error message to display
 */
void displayError(LiquidCrystal_I2C &lcd, const char* errorMsg);

/**
 * @brief Display reset button press message
 * @param lcd LCD object reference
 * @param secondsHeld Seconds button has been held
 */
void displayResetProgress(LiquidCrystal_I2C &lcd,
                          uint8_t secondsHeld);

/**
 * @brief Display system reset confirmation
 * @param lcd LCD object reference
 */
void displaySystemReset(LiquidCrystal_I2C &lcd);

#endif