#include "pinDescription.h"
#include "eepromControl.h"

#define NUM_OF_MAX_COUNT 2000
#define MOTOR_ON_TIME    3000
#define MOTOR_OFF_TIME   27000

void setup()
{
    uint8_t location;
    uint16_t countValue;
    uint16_t countLeft;

    Serial.begin(9600);
    pinMode(MOTOR_PIN, OUTPUT);

    Serial.println("-------------");
    Serial.print("Reading EEPROM:");

    location = getEepromLocationAddress();
    countValue = getTotalCountFromEeprom();
    
    // If count value more than max count then the count is not valid or the
    // location is accessed for the first time, so the default value is 0xFFFF; 
    if(countValue > NUM_OF_MAX_COUNT) 
    {
        clearEepromCounter();
        countValue = 0;
    }

    // If the data is perfetly same as max count then it shows the previously
    // the system reached to the max count and now update the location to point
    // to the next frame for count and reset the count value to 0.
    if (countValue == NUM_OF_MAX_COUNT)
    {
        location = updateEepromAddressForNextCycle();
        clearEepromCounter();
        countLeft = NUM_OF_MAX_COUNT;
        countValue = 0;
    }
    else
    {
        countLeft = NUM_OF_MAX_COUNT - countValue;
    }

    Serial.print("Location:");
    Serial.println(location);
    Serial.print("CountValue:");
    Serial.println(countValue);
    Serial.print("CountLeft:");
    Serial.println(countLeft);

    Serial.println("\nEntering to loop...\n-------------");

    for(int i = countValue + 1; i <= NUM_OF_MAX_COUNT; i++)
    {
        Serial.print("Loop:");
        Serial.println(i);
        digitalWrite(MOTOR_PIN, HIGH);
        Serial.println("Motor High");
        delay(MOTOR_ON_TIME);
        digitalWrite(MOTOR_PIN, LOW);
        Serial.println("Motor Low");
        delay(MOTOR_OFF_TIME);
        updateCountInEeprom();
    }
    Serial.println("I am Done for count!!");
    Serial.println("Please Do a restart for new cycle...");
}

void loop()
{

}