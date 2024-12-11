#include <HapticGlove_ESPNOW.h>
#include <HapticGlove.h>
#include "GloveControl.h"
#include "HapticFeedback.h"
#include "IMU.hpp"
#include "GloveControl.h"
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include "driver/gpio.h"
// General Arduino Code for Demonstration

TaskHandle_t sensorProcessing;
TaskHandle_t hapticControl;

volatile bool ESPNOW_setup;

gpio_config_t myGPIOconfig;

hw_timer_t *Timer0 = NULL;
volatile bool timer0_triggered = false; // Flag to indicate timer interrupt occurrence
hw_timer_t *Timer1 = NULL;
volatile bool timer1_triggered = false; // Flag to indicate timer interrupt occurrence

void IRAM_ATTR Timer0_ISR();
void IRAM_ATTR Timer1_ISR();
void IRAM_ATTR button_0_isr();
void IRAM_ATTR button_1_isr();

uint8_t button_counter;

GloveControlPanel controlPanel = GloveControlPanel(&button_0_isr, &button_1_isr);
volatile bool button_pressed = false;

void triggerGloveControl(){
  sendPositionData();
}

void triggerHapticFeedback(){
  triggerFeedback();
}

void hapticControlCode(void* params){
  Serial.print("Setting up hapticControl from core ");
  Serial.println(xPortGetCoreID());
  setupFeedback();
  uint8_t mac[] = PEER_MAC;
  glove_ESPNOWsetup(mac);
  ESPNOW_setup = true;

  Timer0 = timerBegin(0, 80, true); // timer speed (Hz) = Timer clock speed (Mhz) / prescaler --> 1 MHz
  timerAttachInterrupt(Timer0, &Timer0_ISR, true); // Attach ISR to Timer0
  timerAlarmWrite(Timer0, 1000000/ISR0_FREQ, true); // Set timer to trigger every 1,000,000/ISR0_FREQ microseconds (1 s/f)
  timerAlarmEnable(Timer0); // Enable the timer alarm

  while (1) {
      if (timer0_triggered) { 
          timer0_triggered = false; // Reset the flag
          triggerHapticFeedback();
      }
      if (button_pressed){
        button_counter--;
        setupFeedback();
        if(button_counter == 0){
          button_pressed = false;
        }
      }
      vTaskDelay(10 / portTICK_PERIOD_MS); // Yield CPU for 10 ms to prevent blocking other tasks
  }

}

void sensorProcessingCode(void* params){
  gloveControlSetup();
  while(!ESPNOW_setup){
    vTaskDelay(10 / portTICK_PERIOD_MS); // Yield CPU for 20 ms
  }
  Serial.print("Setting up sensorProcessing from core ");
  Serial.println(xPortGetCoreID());

  Timer1 = timerBegin(1, 80, true); // timer speed (Hz) = Timer clock speed (Mhz) / prescaler --> 1 MHz
  timerAttachInterrupt(Timer1, &Timer1_ISR, true); // Attach ISR to Timer1
  timerAlarmWrite(Timer1, 1000000/ISR1_FREQ, true); // Set timer to trigger every 1,000,000/ISR1_FREQ microseconds (1 s/f)
  timerAlarmEnable(Timer1); // Enable the timer alarm

  while (1) {
      if (timer1_triggered) { 
          timer1_triggered = false; // Reset the flag
          triggerGloveControl();
      }
      if (button_pressed){
        button_counter--;
        // if(IMUS_CONNECTED){
        //   initializeIMUs();
        //   calibrateIMUs();
        // }
        fingerTrackingSetup();
        if(button_counter == 0){
          button_pressed = false;
        }
      }
      vTaskDelay(10 / portTICK_PERIOD_MS); // Yield CPU for 10 ms to prevent blocking other tasks
  }
}

void IRAM_ATTR button_0_isr(){
  return;
}

void IRAM_ATTR button_1_isr(){
  button_pressed = true;
  button_counter = 2;
}

void setup() {
  myGPIOconfig.pin_bit_mask = (0x1);
  myGPIOconfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
  myGPIOconfig.pull_up_en = GPIO_PULLUP_DISABLE;
  myGPIOconfig.mode = GPIO_MODE_OUTPUT;
  myGPIOconfig.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&myGPIOconfig);
  button_counter = 0;
  // Start the Serial Monitor
  int baud_rate = 115200;
  Serial.begin(baud_rate);
  Serial.println("initializing . . .");
  controlPanel.initialize();
  controlPanel.setIntLED(1);
  controlPanel.setIntLEDColor(51, 51, 255); //blue
  ESPNOW_setup = false;
  button_pressed = false;
  Serial.println("Control Panel");
  Serial.println("IMUs");
  // IMU initialization and calibration
  if(IMUS_CONNECTED){
    controlPanel.setIntLED(1);
    controlPanel.setIntLEDColor(255, 0, 0); //red
    Serial.println("init IMUs");
    initializeIMUs();
    controlPanel.setIntLED(1);
    controlPanel.setIntLEDColor(255, 102, 204); // pink
    Serial.println("calibrate IMUs");
    calibrateIMUs(); // pink at wait nonzero and purple at wait button
  }
  controlPanel.setIntLED(1);
  controlPanel.setIntLEDColor(255, 102, 0); // orange
  fingerTrackingSetup();
  controlPanel.setIntLED(0);

      // assign armControl to core 0
    xTaskCreatePinnedToCore(
        hapticControlCode,   // function
        "hapticControl",     // name
        10000,       // stack depth
        NULL,        // parameters (void*)
        1,           // uxPriority
        &hapticControl,      // task handle (tracking)
        0);          // coreID

    // assign pressureSensor to core 1
    xTaskCreatePinnedToCore(
        sensorProcessingCode,   // function
        "sensorProcessing",     // name
        10000,       // stack depth
        NULL,        // parameters (void*)
        1,           // uxPriority
        &sensorProcessing,      // task handle (tracking)
        1);          // coreID
}

// Interrupt Service Routine (ISR) for Timer0
void IRAM_ATTR Timer0_ISR() {
  timer0_triggered = true;  // Set a flag for the task to handle instead of printing directly
}

// Interrupt Service Routine (ISR) for Timer1
void IRAM_ATTR Timer1_ISR() {
  timer1_triggered = true;  // Set a flag for the task to handle instead of printing directly
}

void loop() {}
