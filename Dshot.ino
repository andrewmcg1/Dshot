#include "DshotTeensy.h"

Dshot esc(1200);

void setup()
{
  Serial.begin(115200);
  Serial.print("worked");
  pinMode(13, OUTPUT);

  esc.attach(7);
  esc.write(1046);
}

void loop()
{
  delay(1000000);
  //esc.write(1046);
}

/*
void setup() {
  // put your setup code here, to run once:
 
  esc.attach(7);
  esc.write(0);
}

void loop() {
  int potValue;
  potValue = analogRead(26);   // reads the value of the potentiometer (value between 0 and 1023)
  potValue = map(potValue, 0, 1023, 1000, 1100);   // scale it to use it with the servo library (value between 0 and 180)

  esc.write(potValue);
}
*/