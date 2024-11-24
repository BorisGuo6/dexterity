#include <Arduino.h>
#include <GloveControlPanel.h>

void IRAM_ATTR button_0_isr() {
  Serial.println("Button 0");
}

void IRAM_ATTR button_1_isr() {
  Serial.println("Button 1");
}

GloveControlPanel control_panel = GloveControlPanel(button_0_isr, button_1_isr);

void setup() {
  Serial.begin(115200);
  control_panel.initialize();
}

void loop() {
  control_panel.setIntLED(1);
  delay(500);
  control_panel.setIntLEDColor(255, 0, 0);
  delay(500);
  control_panel.setIntLEDColor(0, 255, 0);
  delay(500);
  control_panel.setIntLEDColor(0, 0, 255);
  delay(500);
  control_panel.toggleIntLED();
  delay(500);
  control_panel.toggleIntLED();
  delay(500);
}
