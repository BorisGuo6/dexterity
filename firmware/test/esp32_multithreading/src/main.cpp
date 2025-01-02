#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

hw_timer_t *Timer0 = NULL;
volatile bool timerTriggered = false; // Flag to indicate timer interrupt occurrence

// Interrupt Service Routine (ISR) for Timer0
void IRAM_ATTR Timer0_ISR() {
  timerTriggered = true;  // Set a flag for the task to handle instead of printing directly
}

void Timer0_Action() {
  Serial.print("Hello from core ");
  Serial.println(xPortGetCoreID()); // Print the core ID outside the ISR
}

// Task handles
TaskHandle_t func1;
TaskHandle_t func2;

void func1code(void* params);
void func2code(void* params);

void setup() {
    Serial.begin(115200);
    Serial.println("initializing . . .");

    // header: BaseType_t xTaskCreatePinnedToCore(TaskFunction_t pvTaskCode, 
    // const char *const pcName, const uint32_t usStackDepth, void *const pvParameters, 
    // UBaseType_t uxPriority, TaskHandle_t *const pvCreatedTask, const BaseType_t xCoreID)

    // assign func1 to core 0
    xTaskCreatePinnedToCore(
        func1code,   // function
        "func1",     // name
        10000,       // stack depth
        NULL,        // parameters (void*)
        2,           // uxPriority
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

// Task function assigned to core 0
void func1code(void* params) {
    Timer0 = timerBegin(0, 80, true); // timer speed (Hz) = Timer clock speed (Mhz) / prescaler --> 1 MHz
    timerAttachInterrupt(Timer0, &Timer0_ISR, true); // Attach ISR to Timer0
    timerAlarmWrite(Timer0, 20000, true); // Set timer to trigger every 20,000 microseconds (20 ms)
    timerAlarmEnable(Timer0); // Enable the timer alarm

    while (1) {
        if (timerTriggered) { 
            timerTriggered = false; // Reset the flag
            Timer0_Action();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Yield CPU for 10 ms to prevent blocking other tasks
    }
}

// Task function assigned to core 1
void func2code(void* params) {
    while (1) {
        Serial.print("Hello from core ");
        Serial.println(xPortGetCoreID()); // Print the core ID
        vTaskDelay(20 / portTICK_PERIOD_MS); // Yield CPU for 20 ms
    }
}

void loop() {
    // Empty loop - not used in this program
}
