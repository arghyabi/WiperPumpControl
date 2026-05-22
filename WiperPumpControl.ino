/**
 * @file WiperPumpControl.ino
 * @brief Wiper pump motor control with EEPROM and LCD display
 *
 * Controls a motor to run 2000 ON/OFF cycles with power-loss recovery.
 * Each cycle: 3 seconds ON, 27 seconds OFF (30 sec total).
 * Progress saved to EEPROM after each cycle for recovery.
 * Status displayed on 20x4 I2C LCD for production use.
 * Serial output maintained for debugging.
 *
 * @author Arghya Biswas
 * @date 2026
 */

#include <LiquidCrystal_I2C.h>
#include "pinDescription.h"
#include "eepromControl.h"
#include "lcdControl.h"
#include "buttonControl.h"

#define NUM_OF_MAX_COUNT 2000  ///< Total cycles per run
#define MOTOR_ON_TIME    3000  ///< Motor ON duration (ms)
#define MOTOR_OFF_TIME   27000 ///< Motor OFF duration (ms)

// Initialize LCD (I2C address 0x27, 20 columns, 4 rows)
LiquidCrystal_I2C lcd(LCD_MODULE_ADDRESS, LCD_MODULE_NO_OF_COLUMN,
                      LCD_MODULE_NO_OF_ROW);

/**
 * @brief Setup function - runs once at power-on or reset
 *
 * Initializes hardware, LCD, reads saved progress from EEPROM,
 * and executes remaining motor cycles. Handles:
 * - LCD display initialization and status updates
 * - Power-loss recovery (resumes from last saved count)
 * - Corrupted count detection and recovery
 * - EEPROM location rotation for wear leveling
 * - Write verification after each cycle
 */
void setup()
{
    uint16_t location;
    uint16_t countValue;
    uint16_t countLeft;

    // Initialize Serial for debugging
    Serial.begin(9600);
    
    // Initialize LCD
    initLCD(lcd);
    displayStartup(lcd);
    
    // Initialize motor pin
    pinMode(MOTOR_PIN, OUTPUT);
    digitalWrite(MOTOR_PIN, LOW);
    
    // Initialize reset button
    initResetButton();

    // Serial debug output
    Serial.println("-------------");
    Serial.println("Wiper Pump Control System");
    Serial.print("Reading EEPROM...");

    // Read EEPROM
    location = getEepromLocationAddress();
    countValue = getTotalCountFromEeprom();
    
    // Flag to track if this is a power-loss recovery
    bool isPowerLossRecovery = false;
    
    // Handle different count scenarios in proper order
    
    // Case 1: Count at max - normal completion, move to next location
    if (countValue == NUM_OF_MAX_COUNT)
    {
        Serial.println("Max count reached, moving to next location...");
        updateEepromAddressForNextCycle();
        location = getEepromLocationAddress();
        clearEepromCounter();
        countValue = 0;
    }
    // Case 2: Count corrupted (too high) - move to next location
    else if(countValue > NUM_OF_MAX_COUNT && countValue != 0xFFFF)
    {
        Serial.println("Corrupted count, moving to next location...");
        updateEepromAddressForNextCycle();
        location = getEepromLocationAddress();
        clearEepromCounter();
        countValue = 0;
    }
    // Case 3: Uninitialized EEPROM - reset at current location
    else if(countValue == 0xFFFF)
    {
        Serial.println("Uninitialized EEPROM, resetting...");
        clearEepromCounter();
        countValue = 0;
    }
    // Case 4: Valid count between 1 and MAX - power loss recovery
    else if(countValue > 0 && countValue < NUM_OF_MAX_COUNT)
    {
        isPowerLossRecovery = true;
        Serial.println("POWER LOSS DETECTED!");
        Serial.print("Resuming from cycle: ");
        Serial.println(countValue);
        
        // Display recovery message on LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Power Loss Detected");
        lcd.setCursor(0, 1);
        lcd.print("Resuming from: ");
        lcd.print(countValue);
        lcd.setCursor(0, 2);
        lcd.print("Remaining: ");
        lcd.print(NUM_OF_MAX_COUNT - countValue);
        delay(3000); // Show message for 3 seconds
    }
    // Case 5: Count is 0 - fresh start (from init or completion)
    // This happens after Case 1 (completion) or Case 3 (init)
    // No special handling needed, just continue
    
    countLeft = NUM_OF_MAX_COUNT - countValue;

    // Serial debug info (detailed)
    Serial.print("Location: ");
    Serial.println(location);
    Serial.print("CountValue: ");
    Serial.println(countValue);
    Serial.print("CountLeft: ");
    Serial.println(countLeft);
    
    if(isPowerLossRecovery)
    {
        Serial.println("\n*** RECOVERING FROM POWER LOSS ***");
    }
    
    Serial.println("\nEntering to loop...\n-------------");

    // Main cycle loop
    for(uint16_t i = countValue + 1; i <= NUM_OF_MAX_COUNT; i++)
    {
        // Check for reset button at start of each cycle
        if(checkResetButton(lcd))
        {
            Serial.println("SYSTEM RESET REQUESTED!");
            displaySystemReset(lcd);
            delay(2000);
            
            // Clear EEPROM counter
            clearEepromCounter();
            Serial.println("Counter cleared to 0");
            Serial.println("Restarting system...");
            delay(1000);
            
            // Software reset using watchdog or jump to 0
            asm volatile ("jmp 0");
        }
        
        // Update LCD with current cycle being executed
        displayCount(lcd, i, NUM_OF_MAX_COUNT);
        
        // Serial debug - shows cycle being executed
        Serial.print("Loop: ");
        Serial.println(i);
        
        // Turn motor ON
        digitalWrite(MOTOR_PIN, HIGH);
        displayMotorStatus(lcd, true);
        Serial.println("Motor High");
        delay(MOTOR_ON_TIME);
        
        // Turn motor OFF
        digitalWrite(MOTOR_PIN, LOW);
        displayMotorStatus(lcd, false);
        Serial.println("Motor Low");
        delay(MOTOR_OFF_TIME);
        
        // Save progress to EEPROM AFTER cycle completes
        // This ensures count only increments for completed cycles
        updateCountInEeprom();
        
        // Verify write (debug only)
        uint16_t verifyCount = getTotalCountFromEeprom();
        if(verifyCount != i)
        {
            Serial.print("WARNING: EEPROM write failed! Exp: ");
            Serial.print(i);
            Serial.print(", Got: ");
            Serial.println(verifyCount);
            displayError(lcd, "EEPROM Error");
            delay(2000);
        }
    }
    
    // Display completion on LCD
    displayComplete(lcd);
    
    // Serial debug
    Serial.println("\n-------------");
    Serial.println("Cycle Complete!");
    Serial.println("I am Done for count!!");
    Serial.println("Please Do a restart for new cycle...");
    Serial.println("-------------");
}

/**
 * @brief Main loop function - runs continuously
 *
 * After cycles complete, monitors reset button to allow
 * manual restart without power cycling.
 */
void loop()
{
    // After all cycles complete, check for reset button
    // to allow restart without power cycle
    if(checkResetButton(lcd))
    {
        Serial.println("SYSTEM RESTART REQUESTED!");
        displaySystemReset(lcd);
        delay(2000);
        
        Serial.println("Restarting system...");
        delay(1000);
        
        // Software reset - system will restart and handle
        // count=2000 case (move to next location)
        asm volatile ("jmp 0");
    }
    
    delay(100); // Small delay to reduce CPU usage
}