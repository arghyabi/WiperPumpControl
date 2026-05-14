#include "pinDescription.h"

void setup()
{
  Serial.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);

  for(int i = 0; i < 2000; i++)
  {
    Serial.print("Loop:");
    Serial.println(i);
    digitalWrite(MOTOR_PIN, HIGH);
    Serial.println("Motor High");
    delay(3000);
    digitalWrite(MOTOR_PIN, LOW);
    Serial.println("Motor Low");
    delay(27000);
  }
  Serial.println("I am Done for 2000 times!!");
}

void loop()
{

}