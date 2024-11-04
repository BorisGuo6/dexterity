#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

TaskHandle_t func1;
TaskHandle_t func2;

void func1code(void* params);
void func2code(void* params);

void setup() {
  Serial.begin(115200);

  // header: BaseType_t xTaskCreatePinnedToCore(TaskFunction_t pvTaskCode, 
  // const char *constpcName, const uint32_t usStackDepth, void *constpvParameters, 
  // UBaseType_t uxPriority, TaskHandle_t *constpvCreatedTask, const BaseType_t xCoreID)

  // assign func1 to core 0
  xTaskCreatePinnedToCore(
      func1code,   // function
      "func1",     // name
      10000,       // stack depth
      NULL,        // parameters (void*)
      1,           // uxPriority
      &func1,      // task handle (tracking)
      0);          // coreID
  delay(500); 

  // assign func2 to core 1
  xTaskCreatePinnedToCore(
      func2code,   // function
      "func2",     // name
      10000,       // stack depth
      NULL,        // parameters (void*)
      1,           // uxPriority
      &func2,      // task handle (tracking)
      1);          // coreID
    delay(500); 
}

void func1code(void* params){
  while(1){
    Serial.print("Hello from core ");
    Serial.println(xPortGetCoreID());
    delay(2000);
  }
}

void func2code(void* params){
  while(1){
    Serial.print("Hello from core ");
    Serial.println(xPortGetCoreID());
    delay(3000);
  }
}

void loop() {
  
}