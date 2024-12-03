#include <ArmControl.h>

int i;

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
    i = 1;
  }
  i += 1;
}

void printServoPositions(){
  printEmaxServoPositions();

}
