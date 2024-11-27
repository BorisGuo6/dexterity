#include "HapticFeedback.h"

int i;

void setupFeedback(){
    i = 1;
    return;
}

void triggerFeedback(){
  if(i % 200 == 0 && ENABLE_ESPNOW_PRINT){
    glove_monitorSuccess();
    i = 1;
  }
  i += 1;
}