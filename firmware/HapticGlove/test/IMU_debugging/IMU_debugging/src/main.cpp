#include "IMU.hpp"


// _____________ SETUP ____________________
void setup()
{
  // initialize serial
  Serial.begin(SERIAL_BAUD);            
  Serial.println("\nRunning...");

  initializeIMUs();

  calibrateIMUs();
}


// _____________ LOOP ____________________
void loop()
{
  // check if cal button is pushed
  //imu_cal_button_state = digitalRead(IMU_CAL_BUTTON);
  //if (imu_cal_button_state == HIGH)
  //{
  //  Serial.println("__________BUTTON PRESSED__________");
  //  set_cal_quat(0);
  //  set_cal_quat(1);
  //}

  // check for reports from both IMUs
  //check_report(0);
  //check_report(1);
  //output_data();  

  euler_t ypr_diff_cal = getAngleDifference();

  Serial.print("E_DIFF_CAL: ");
  Serial.print(ypr_diff_cal.yaw);      Serial.print(" ");
  Serial.print(ypr_diff_cal.pitch);    Serial.print(" ");
  Serial.print(ypr_diff_cal.roll);     
  Serial.println();
}

