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
    Serial.print("Location:");
    Serial.println(location);
    countValue = getTotalCountFromEeprom();
    Serial.print("CountValue:");
    Serial.println(countValue);
    
    if (countValue == NUM_OF_MAX_COUNT)
    {
        updateEepromAddressForNextCycle();
        countLeft = NUM_OF_MAX_COUNT;
    }
    else
    {
        countLeft = NUM_OF_MAX_COUNT - countValue;
    }
    Serial.print("CountLeft:");
    Serial.println(countLeft);

    Serial.println("\nEntering to loop...\n-------------");

    for(int i = 0; i < countLeft; i++)
    {
        Serial.print("Loop:");
        Serial.println(i);
        digitalWrite(MOTOR_PIN, HIGH);
        Serial.println("Motor High");
        delay(MOTOR_ON_TIME);
        digitalWrite(MOTOR_PIN, LOW);
        Serial.println("Motor Low");
        delay(MOTOR_OFF_TIME);
    }
    Serial.println("I am Done for count!!");
    Serial.println("Please Do a restart for new cycle...");
}

void loop()
{

}