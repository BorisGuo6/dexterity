#include "RoboticArm.h"
#include "PressureSensors.h"
#include "ArmControl.h"

TaskHandle_t pressureSensor;
TaskHandle_t armControl;
hw_timer_t *Timer0;
volatile bool timerTriggered;

void IRAM_ATTR pressureSensorISR()
{
  timerTriggered = true;
}

// call pollPressureSensors -->
void pressureSensorISR_Action(){
  // TODO: Write code to poll force sensors, handle and send data
  sendPressureData();
}

// core 0
// call arm_ESPNOWsetup
// call armControlSetup
void armControlCode(void* params){
  uint8_t peer_mac[] = PEER_MAC;
  //setup ESPNOW
	arm_ESPNOWsetup(peer_mac);
  armControlSetup();
}

// core 1
// set up 50 Hz ISR, call pressureSensorISR_Action when ISR is triggered
void pressureSensorCode(void* params){
  // Set up Timer-Based ISR
  Timer0 = timerBegin(0, 80, true); // timer speed (Hz) = Timer clock speed (Mhz) / prescaler --> 1 MHz
  timerAttachInterrupt(Timer0, &pressureSensorISR, true); // Attach ISR to Timer0
  timerAlarmWrite(Timer0, 20000, true); // Set timer to trigger every 20,000 microseconds (20 ms)
  timerAlarmEnable(Timer0); // Enable the timer alarm

  while(1){
        if (timerTriggered) { 
            timerTriggered = false; // Reset the flag
            pressureSensorISR_Action();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Yield CPU for 10 ms to prevent blocking other tasks
  }
}

// initialize UART0 w/ 115200 baud
// start pressureSensorCode in core 1
// start armControlCode in core 0
void setup() {
  Serial.begin(115200);
  Serial.println("initializing . . .");

  Timer0 = NULL;
  timerTriggered = false; // Flag to indicate timer interrupt occurrence
  
  xTaskCreatePinnedToCore(
      pressureSensorCode,   // function
      "pressureSensor",     // name
      10000,       // stack depth
      NULL,        // parameters (void*)
      1,           // uxPriority
      &pressureSensor,      // task handle (tracking)
      1);          // coreID

  xTaskCreatePinnedToCore(
      armControlCode,   // function
      "armControl",     // name
      10000,       // stack depth
      NULL,        // parameters (void*)
      1,           // uxPriority
      &armControl,      // task handle (tracking)
      0);          // coreID
}

// don't do anything
void loop() {

}