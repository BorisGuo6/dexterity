#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"
#include "haptic_glove_pins.h"

TaskHandle_t sensorProcessing;
TaskHandle_t hapticControl;

void sensorProcessingCode(void* params);
void hapticControlCode(void* params);

void setup() {
  Serial.begin(115200);
  Serial.println("initializing . . .");

  // header: BaseType_t xTaskCreatePinnedToCore(TaskFunction_t pvTaskCode, 
  // const char *constpcName, const uint32_t usStackDepth, void *constpvParameters, 
  // UBaseType_t uxPriority, TaskHandle_t *constpvCreatedTask, const BaseType_t xCoreID)

  // assign func1 to core 0
  xTaskCreatePinnedToCore(
      sensorProcessingCode,   // function
      "sensorProcessing",     // name
      10000,       // stack depth
      NULL,        // parameters (void*)
      1,           // uxPriority
      &sensorProcessing,      // task handle (tracking)
      0);          // coreID
  delay(500); 

  // assign func2 to core 1
  xTaskCreatePinnedToCore(
      hapticControlCode,   // function
      "hapticControl",     // name
      10000,       // stack depth
      NULL,        // parameters (void*)
      1,           // uxPriority
      &hapticControl,      // task handle (tracking)
      1);          // coreID
    delay(500); 
}

void sensorProcessingCode(void* params){
  // Set up Timer-Based ISR
  Serial.println("Set Up Timer-Based ISR ~50-100 Hz . . .");
  delay(500);
  // Write ISR code to read sensors and send data to arm
  Serial.println("Poll Sensors and Send Data.");
  delay(500);
  // Sanity check to make sure code is running in correct core
  Serial.print("Sensor Processing Code running in Core ");
  Serial.println(xPortGetCoreID());
}

void hapticControlCode(void* params){
  // register ESPNOW callback func
  Serial.println("Register ESPNOW callback . . .");
  delay(500);
  // Write ESPNOW callback function to process data and control haptics
  Serial.println("Process Haptic Data in ESPNOW callback.");
  delay(500);
  // Sanity check to make sure code is running in correct core
  Serial.print("Haptic Control Code running in Core ");
  Serial.println(xPortGetCoreID());
}

void loop() {
  // arduino requirement i think
}