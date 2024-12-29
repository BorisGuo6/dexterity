#include <Arduino.h>
#include <General_ESPNOW.h>

#define FSR_TEST_PIN 4

int fsr;

void setup() {
  
  Serial.begin(115200);
}


void loop() {
  
  fsr = analogRead(FSR_TEST_PIN);
  //send data to Glove ESP

  Serial.print("ADC value: ");
  Serial.println(fsr);
}
