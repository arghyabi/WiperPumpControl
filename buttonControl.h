/**
 * @file buttonControl.h
 * @brief Reset button control and monitoring
 */

#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define RESET_HOLD_TIME 5000  ///< Reset button hold time (ms)

/**
 * @brief Initialize reset button pin
 * 
 * Configures reset button pin as INPUT_PULLUP for active LOW.
 */
void initResetButton();

/**
 * @brief Check if reset button is held for 5 seconds
 * 
 * Monitors reset button and shows countdown on LCD.
 * Returns true if button held for full 5 seconds.
 * 
 * @param lcd LCD object reference for displaying countdown
 * @return true if reset requested, false otherwise
 */
bool checkResetButton(LiquidCrystal_I2C &lcd);

#endif

// Made with Bob
