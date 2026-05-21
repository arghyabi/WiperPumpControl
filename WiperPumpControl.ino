/**
 * @file WiperPumpControl.ino
 * @brief Wiper pump motor control with EEPROM-based cycle counting
 *
 * Controls a motor to run 2000 ON/OFF cycles with power-loss recovery.
 * Each cycle: 3 seconds ON, 27 seconds OFF (30 sec total).
 * Progress saved to EEPROM after each cycle for recovery.
 *
 * @author Your Name
 * @date 2026
 */

#include "pinDescription.h"
#include "eepromControl.h"

#define NUM_OF_MAX_COUNT 2000  ///< Total cycles per run
#define MOTOR_ON_TIME    3000  ///< Motor ON duration (ms)
#define MOTOR_OFF_TIME   27000 ///< Motor OFF duration (ms)

/**
 * @brief Setup function - runs once at power-on or reset
 *
 * Initializes hardware, reads saved progress from EEPROM,
 * and executes remaining motor cycles. Handles:
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

    Serial.begin(9600);
    pinMode(MOTOR_PIN, OUTPUT);
    
    // Ensure motor is off initially
    digitalWrite(MOTOR_PIN, LOW);

    Serial.println("-------------");
    Serial.println("Wiper Pump Control System");
    Serial.print("Reading EEPROM...");

    location = getEepromLocationAddress();
    countValue = getTotalCountFromEeprom();
    
    // Handle different count scenarios in proper order
    
    // Case 1: Count at max - normal completion, move to next location
    if (countValue == NUM_OF_MAX_COUNT)
    {
        Serial.println("Max count reached, moving to next location...");
        updateEepromAddressForNextCycle();
        
        // Re-validate location after update
        location = getEepromLocationAddress();
        clearEepromCounter();
        countValue = 0;
    }
    // Case 2: Count corrupted (too high) - move to next location
    else if(countValue > NUM_OF_MAX_COUNT && countValue != 0xFFFF)
    {
        Serial.println("Corrupted count, moving to next location...");
        updateEepromAddressForNextCycle();
        
        // Re-validate location after update
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
    // Case 4: Valid count (0 to NUM_OF_MAX_COUNT-1) - continue
    // No action needed, just calculate remaining count
    
    countLeft = NUM_OF_MAX_COUNT - countValue;

    Serial.print("Location: ");
    Serial.println(location);
    Serial.print("CountValue: ");
    Serial.println(countValue);
    Serial.print("CountLeft: ");
    Serial.println(countLeft);

    Serial.println("\nEntering to loop...\n-------------");

    // Use uint16_t to prevent overflow
    for(uint16_t i = countValue + 1; i <= NUM_OF_MAX_COUNT; i++)
    {
        Serial.print("Loop: ");
        Serial.println(i);
        
        // Turn motor ON
        digitalWrite(MOTOR_PIN, HIGH);
        Serial.println("Motor High");
        delay(MOTOR_ON_TIME);
        
        // Turn motor OFF
        digitalWrite(MOTOR_PIN, LOW);
        Serial.println("Motor Low");
        delay(MOTOR_OFF_TIME);
        
        // Save progress to EEPROM after each successful cycle
        updateCountInEeprom();
        
        // Verify the write was successful
        uint16_t verifyCount = getTotalCountFromEeprom();
        if(verifyCount != i)
        {
            Serial.print("WARNING: EEPROM write failed! Exp: ");
            Serial.print(i);
            Serial.print(", Got: ");
            Serial.println(verifyCount);
        }
    }
    
    Serial.println("\n-------------");
    Serial.println("Cycle Complete!");
    Serial.println("I am Done for count!!");
    Serial.println("Please Do a restart for new cycle...");
    Serial.println("-------------");
}

/**
 * @brief Main loop function - runs continuously
 *
 * Empty by design. All motor control logic executes in setup()
 * to ensure cycles complete before allowing restart.
 */
void loop()
{
    // Intentionally empty - all logic in setup()
}