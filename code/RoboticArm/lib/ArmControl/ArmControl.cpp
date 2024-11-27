#include <ArmControl.h>

void armControlSetup() {
  //setup Finger Control Library
  fingerControlSetup();
  wristControlSetup();

  Serial.println("ARM CONTROL: Missing dependencies for ArmControlPanel, WristControl");
}

void controlArm(){
  controlFingers(arm_inData.finger_pos);
  controlWrist(arm_inData.wrist_pos);
  if(ENABLE_CONTROL_PRINT){
    printServoPositions();
  }
}

void printServoPositions(){
  printEmaxServoPositions();

}
