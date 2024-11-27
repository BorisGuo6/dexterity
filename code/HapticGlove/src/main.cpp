#include <HapticGlove_ESPNOW.h>
#include <HapticGlove.h>
// General Arduino Code for Demonstration

int i;

TaskHandle_t sensorProcessing;
TaskHandle_t hapticControl;

volatile bool ESPNOW_setup;

hw_timer_t *Timer0 = NULL;
volatile bool timerTriggered = false; // Flag to indicate timer interrupt occurrence

void IRAM_ATTR Timer0_ISR();

void triggerSensorProcessing(){
  uint8_t fpos[16];
  uint8_t wpos[3];
  uint8_t apos[3];
  for(int j=0; j<16; j++){
    fpos[j] = random(1, 255);
  }
  for(int j=0; j<3; j++){
    wpos[j] = random(1, 255);
    apos[j] = random(1, 255);
  }
  glove_sendData(fpos, wpos, apos);
}

void hapticControlCode(void* params){
  Serial.print("Setting up hapticControl from core ");
  Serial.println(xPortGetCoreID());
  uint8_t mac[] = PEER_MAC;
  glove_ESPNOWsetup(mac);
  ESPNOW_setup = true;
  i = 0;
  while(1){
    i += 1;
    vTaskDelay(20 / portTICK_PERIOD_MS);
    if(i % 200 == 0 && ENABLE_ESPNOW_PRINT){
      glove_monitorSuccess();
    }
  }
}

void sensorProcessingCode(void* params){
  while(!ESPNOW_setup){
    vTaskDelay(10 / portTICK_PERIOD_MS); // Yield CPU for 20 ms
  }
  Serial.print("Setting up sensorProcessing from core ");
  Serial.println(xPortGetCoreID());

  Timer0 = timerBegin(0, 80, true); // timer speed (Hz) = Timer clock speed (Mhz) / prescaler --> 1 MHz
  timerAttachInterrupt(Timer0, &Timer0_ISR, true); // Attach ISR to Timer0
  timerAlarmWrite(Timer0, 1000000/ISR_FREQ, true); // Set timer to trigger every 1,000,000/ISR_FREQ microseconds (1 s/f)
  timerAlarmEnable(Timer0); // Enable the timer alarm

  while (1) {
      if (timerTriggered) { 
          timerTriggered = false; // Reset the flag
          triggerSensorProcessing();
      }
      vTaskDelay(10 / portTICK_PERIOD_MS); // Yield CPU for 10 ms to prevent blocking other tasks
  }
}

void setup() {
  // Start the Serial Monitor
  int baud_rate = 115200;
  Serial.begin(baud_rate);
  Serial.println("initializing . . .");
  ESPNOW_setup = false;

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
  timerTriggered = true;  // Set a flag for the task to handle instead of printing directly
}

void loop() {}
