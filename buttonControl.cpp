/**
 * @file buttonControl.cpp
 * @brief Implementation of reset button control functions
 */

#include "buttonControl.h"
#include "pinDescription.h"
#include "lcdControl.h"

/**
 * @brief Initialize reset button pin
 * 
 * Configures reset button pin as INPUT_PULLUP.
 * Button is active LOW (pressed = LOW, released = HIGH).
 */
void initResetButton()
{
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
}

/**
 * @brief Check if reset button is held for 5 seconds
 * 
 * Monitors reset button state and displays countdown on LCD.
 * If button is held continuously for 5 seconds, returns true.
 * If button is released before 5 seconds, returns false.
 * 
 * Uses 50ms polling interval for debouncing.
 * 
 * @param lcd LCD object reference for displaying progress
 * @return true if button held for full 5 seconds, false otherwise
 */
bool checkResetButton(LiquidCrystal_I2C &lcd)
{
    // Check if button is pressed (active LOW)
    if(digitalRead(RESET_BUTTON_PIN) == LOW)
    {
        unsigned long pressStart = millis();
        uint8_t lastSecond = 0;
        
        // Monitor button while it's held down
        while(digitalRead(RESET_BUTTON_PIN) == LOW)
        {
            unsigned long elapsed = millis() - pressStart;
            uint8_t secondsHeld = elapsed / 1000;
            
            // Update LCD display every second
            if(secondsHeld != lastSecond && secondsHeld < 5)
            {
                displayResetProgress(lcd, secondsHeld);
                lastSecond = secondsHeld;
                
                // Serial debug output
                Serial.print("Reset button held: ");
                Serial.print(secondsHeld);
                Serial.println(" seconds");
            }
            
            // Check if button held for full 5 seconds
            if(elapsed >= RESET_HOLD_TIME)
            {
                Serial.println("Reset button held for 5 seconds!");
                return true;
            }
            
            delay(50); // Small delay for debouncing
        }
        
        // Button was released before 5 seconds
        Serial.println("Reset cancelled - button released");
        return false;
    }
    
    return false;
}

// Made with Bob
