#include <RoboticArm_ESPNOW.h>

// General Arduino Code for Demonstration

int i;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  arm_ESPNOWsetup(1);
  i = 0;
}

void loop() {

  arm_sendData(random(1, 100), random(1, 100), random(1, 100), random(1, 100), random(1, 100));

  delay(1000/DATA_RATE);  // Wait before sending again

  i += 1;
  if(i % 200 == 0){
    arm_monitorSuccess();
  }
}