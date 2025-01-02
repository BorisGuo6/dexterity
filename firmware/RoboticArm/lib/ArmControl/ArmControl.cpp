#include <ArmControl.h>

uint8_t i;
unsigned long time0_elapsed;

void armControlSetup() {
  //setup Finger Control Library
  fingerControlSetup();
  wristControlSetup();

  Serial.println("ARM CONTROL: Missing dependencies for ArmControlPanel, WristControl");
  i = 1;
}

void controlArm(){
  controlFingers(arm_inData.finger_pos);
  controlWrist(arm_inData.wrist_pos);
  if(ENABLE_CONTROL_PRINT){
    printServoPositions();
  }
  if(i % 200 == 0 && ENABLE_ESPNOW_PRINT){
    arm_monitorSuccess();
    if(!TRACK_ISR_0) i = 1;
  }
  if(i % 200 == 0 && TRACK_ISR_0){
    Serial.print("\ncallback 0 run at "); Serial.print((200*1000000)/(micros() - time0_elapsed)); Serial.println(" Hz\n");
    i = 1;
    time0_elapsed = micros();
  }
  i++;
}

void printServoPositions(){
  printEmaxServoPositions();

}
