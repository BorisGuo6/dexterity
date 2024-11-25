#include <Arduino.h>
#include <GloveControlPanel.h>

bool button_0_pressed;
bool button_1_pressed;

void IRAM_ATTR button_0_isr() {
  button_0_pressed = true;
}

void IRAM_ATTR button_1_isr() {
  button_1_pressed = true;
}

GloveControlPanel control_panel = GloveControlPanel(button_0_isr, button_1_isr);

void setup() {
  Serial.begin(115200);
  control_panel.initialize();
}

void loop() {
  if (button_0_pressed) {
    control_panel.toggleIntLED();
    button_0_pressed = false;
    delay(100);
  } /*else if (button_1_pressed) {
    Serial.print("Button 1\n");
    button_1_pressed = false;
  }*/
}
