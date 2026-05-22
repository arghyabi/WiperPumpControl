/**
 * @file lcdControl.cpp
 * @brief Implementation of LCD display functions for 20x4 I2C LCD
 */

#include <LiquidCrystal_I2C.h>
#include "lcdControl.h"
#include "pinDescription.h"

/**
 * @brief Initialize LCD display
 *
 * Sets up the LCD with backlight on and clears the screen.
 *
 * @param lcd LCD object reference
 */
void initLCD(LiquidCrystal_I2C &lcd)
{
    lcd.begin(LCD_MODULE_NO_OF_COLUMN, LCD_MODULE_NO_OF_ROW);
    lcd.backlight();
    lcd.clear();
}

/**
 * @brief Display system startup message
 *
 * Shows welcome message on LCD during initialization.
 *
 * @param lcd LCD object reference
 */
void displayStartup(LiquidCrystal_I2C &lcd)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wiper Pump Control");
    lcd.setCursor(0, 1);
    lcd.print("System Starting...");
    delay(2000);
}

/**
 * @brief Display current cycle count and remaining
 *
 * Shows current progress on LCD:
 * Line 0: Current cycle / Total cycles
 * Line 1: Remaining cycles
 *
 * @param lcd LCD object reference
 * @param current Current cycle number (1-2000)
 * @param total Total cycles to complete (2000)
 */
void displayCount(LiquidCrystal_I2C &lcd, uint16_t current,
                  uint16_t total)
{
    uint16_t remaining = total - current;
    
    // Line 0: Current / Total
    lcd.setCursor(0, 0);
    lcd.print("Cycle: ");
    lcd.print(current);
    lcd.print("/");
    lcd.print(total);
    lcd.print("    "); // Clear extra chars
    
    // Line 1: Remaining
    lcd.setCursor(0, 1);
    lcd.print("Remaining: ");
    lcd.print(remaining);
    lcd.print("    "); // Clear extra chars
}

/**
 * @brief Display motor status
 *
 * Shows motor ON/OFF status on line 2 of LCD.
 *
 * @param lcd LCD object reference
 * @param isOn true if motor is ON, false if OFF
 */
void displayMotorStatus(LiquidCrystal_I2C &lcd, bool isOn)
{
    lcd.setCursor(0, 2);
    if(isOn)
    {
        lcd.print("Motor: ON           ");
    }
    else
    {
        lcd.print("Motor: OFF          ");
    }
}

/**
 * @brief Display completion message
 *
 * Shows completion message when all cycles are done.
 *
 * @param lcd LCD object reference
 */
void displayComplete(LiquidCrystal_I2C &lcd)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  CYCLE COMPLETE!  ");
    lcd.setCursor(0, 1);
    lcd.print("  All 2000 cycles  ");
    lcd.setCursor(0, 2);
    lcd.print("    completed!     ");
    lcd.setCursor(0, 3);
    lcd.print(" Restart to begin ");
}

/**
 * @brief Display error message
 *
 * Shows error message on LCD for debugging.
 *
 * @param lcd LCD object reference
 * @param errorMsg Error message to display (max 20 chars)
 */
void displayError(LiquidCrystal_I2C &lcd, const char* errorMsg)
{
    lcd.setCursor(0, 3);
    lcd.print("ERR:");
    lcd.print(errorMsg);
    lcd.print("          "); // Clear extra chars
}

/**
 * @brief Display reset button press progress
 *
 * Shows countdown while reset button is held.
 *
 * @param lcd LCD object reference
 * @param secondsHeld Seconds button has been held (1-5)
 */
void displayResetProgress(LiquidCrystal_I2C &lcd,
                          uint8_t secondsHeld)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reset Button Held");
    lcd.setCursor(0, 1);
    lcd.print("Hold for: ");
    lcd.print(5 - secondsHeld);
    lcd.print(" sec");
    lcd.setCursor(0, 2);
    lcd.print("Release to cancel");
}

/**
 * @brief Display system reset confirmation
 *
 * Shows message when system is being reset.
 *
 * @param lcd LCD object reference
 */
void displaySystemReset(LiquidCrystal_I2C &lcd)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  SYSTEM RESET!  ");
    lcd.setCursor(0, 1);
    lcd.print("Clearing counter...");
    lcd.setCursor(0, 2);
    lcd.print("Restarting system");
}