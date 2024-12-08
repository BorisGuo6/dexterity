#include "HapticFeedback.h"

uint8_t k;
unsigned long time0_elapsed;

void setupFeedback(){
    k = 1;
    if(TRACK_ISR_0) time0_elapsed = micros();
}

void triggerFeedback(){
  if(k % 200 == 0 && ENABLE_ESPNOW_PRINT){
    glove_monitorSuccess();
    k = 1;
  }
  else if(k % 200 == 0 && TRACK_ISR_0){
    Serial.print("\ncallback 0 run at "); Serial.print((200*1000000)/(micros() - time0_elapsed)); Serial.println(" Hz\n");
    k = 1;
    time0_elapsed = micros();
  }
  k++;
}