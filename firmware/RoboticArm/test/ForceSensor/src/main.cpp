#include <Arduino.h>

#define FSR_PIN 1

int pressure_reading = 0;

void setup() {
  
  Serial.begin(115200);
}

void loop() {
  
  pressure_reading = analogRead(FSR_PIN);

  Serial.print("Analog value: ");
  Serial.println(pressure_reading);
  delay(200);
}
