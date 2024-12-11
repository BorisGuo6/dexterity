#include <Arduino.h>
#include <HapticFeedback.h>

HapticFeedback haptic_feedback;

void setup() {
  Serial.begin(115200);
  haptic_feedback = HapticFeedback();
  haptic_feedback.setupFeedback();
}

void loop() {
  
}
