#include "GloveControl.h"

void gloveControlSetup(){
    return;
}

void sendPositionData(){
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