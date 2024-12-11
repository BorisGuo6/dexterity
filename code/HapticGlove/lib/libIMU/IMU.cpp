#include "IMU.hpp"


// _________________ GLOBAL VAR DEFINITIONS ____________________
iquat_t iq[BNOs];           // int quaternion, reported by sensor                                                              
fquat_t fq[BNOs];           // float quaternion, converted from int quat
fquat_t fq_ref[BNOs];       // reference quaternions for calibration

int imu_cal_button_state = BUTTON_NORMAL;

TwoWire IMUWire = TwoWire(TWOWIRE_INSTANCE_ADDRESS); 


// _________________ FUNCTION DEFINITIONS ______________________
void initializeIMUs()
{
  Serial.println("initializeIMUs");
  // reset all BNOs
  pinMode(pinRST,OUTPUT);               
  digitalWrite(pinRST,LOW);
  delay(1);
  digitalWrite(pinRST,HIGH);
  delay(300);

  // initialize I2C
  IMUWire.begin(IMU_I2C_SDA, IMU_I2C_SCL);                         
  IMUWire.setClock(I2C_CLOCK);

  // request desired reports
  request_reports(0);
  request_reports(1);
}

void calibrateIMUs()
{
  Serial.println("calibrateIMUs");
  // set pin for calibration button
  pinMode(IMU_CAL_BUTTON, INPUT);

  // get IMU reports until both quats are non zero
  Serial.println("check nonzero");
  while (isIntQuatZero(0) || isIntQuatZero(1))
  {
    check_report(0);
    check_report(1);
  }

  Serial.println("wait for button");
  // get reports until calibration button is pressed
  controlPanel.setIntLED(1);
  controlPanel.setIntLEDColor(102, 51, 204);
  while (imu_cal_button_state == BUTTON_NORMAL) 
  {
    check_report(0);
    check_report(1);

    // poll calibration button
    imu_cal_button_state = digitalRead(IMU_CAL_BUTTON);
    Serial.println(digitalRead(IMU_CAL_BUTTON));
  }

  // set float quaternions
  set_fquat(0);
  set_fquat(1);

  // set calibration quaternions
  set_cal_quat(0);
  set_cal_quat(1);
}

bool isIntQuatZero(uint8_t bno)
{
  iquat_t *iquat = &(iq[bno]);

  return (
    iquat->qr == 0
    &&
    iquat->qi == 0
    &&
    iquat->qj == 0
    &&
    iquat->qk == 0
  );
}

euler_t getAngleDifference()
{
  // check reports first to get new quats
  check_report(0);
  check_report(1);

  // set float quats
  set_fquat(0);
  set_fquat(1);

  // get calibrated quaternions for each sensor
  fquat_t fq0_cal = differenceQuat(&(fq[0]), &(fq_ref[0]));
  fquat_t fq1_cal = differenceQuat(&(fq[1]), &(fq_ref[1]));

  // get difference quat and angle
  fquat_t fq_diff_cal = differenceQuat(&fq1_cal, &fq0_cal);
  euler_t ypr_diff_cal = quaternionToEuler(&fq_diff_cal, true);

  return ypr_diff_cal;
}

void set_fquat(uint8_t bno)
{
  iquat_t *iquat = &(iq[bno]);
  fquat_t *fquat = &(fq[bno]);
  fquat->qr = iquat->qr * SCALE_Q(8);
  fquat->qi = iquat->qi * SCALE_Q(8);
  fquat->qj = iquat->qj * SCALE_Q(8);
  fquat->qk = iquat->qk * SCALE_Q(8);
  normalizeQuaternion(fquat);
}

void set_cal_quat(uint8_t bno)
{
  fquat_t *fquat = &(fq[bno]);
  fquat_t *fquat_ref = &(fq_ref[bno]);
  fquat_ref->qr = fquat->qr;
  fquat_ref->qi = fquat->qi;
  fquat_ref->qj = fquat->qj;
  fquat_ref->qk = fquat->qk;
}

uint8_t get_bno_addr(uint8_t bno)
{
  uint8_t BNO_ADDR;
  if (bno == 1) 
  {
    BNO_ADDR = BNO_ADDR2;
  }
  else 
  {
    BNO_ADDR = BNO_ADDR1;
  }
  return BNO_ADDR;
}

static void request_reports(uint8_t bno)
{ 
  uint8_t BNO_ADDR = get_bno_addr(bno);
  
  if (REQUEST_ACC_REPORTS)
  {
    // request acc reports, see 6.5.4
    static const uint8_t cmd_acc[]  = {21, 0, 2, 0, 0xFD, ACC_REPORT_REQUEST_ID,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    IMUWire.beginTransmission(BNO_ADDR);  IMUWire.write(cmd_acc, sizeof(cmd_acc));  IMUWire.endTransmission();
  }

  if (REQUEST_GYRO_REPORTS)
  {
    // request gyro reports, see 6.5.4
    static const uint8_t cmd_gyro[] = {21, 0, 2, 0, 0xFD, GYRO_REPORT_REQUEST_ID, 0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    IMUWire.beginTransmission(BNO_ADDR);  IMUWire.write(cmd_gyro, sizeof(cmd_gyro));  IMUWire.endTransmission();
  }

  if (REQUEST_MAG_REPORTS)
  {
    // request magneto reports, see 6.5.4
    static const uint8_t cmd_mag[]  = {21, 0, 2, 0, 0xFD, MAG_REPORT_REQUEST_ID,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    IMUWire.beginTransmission(BNO_ADDR);  IMUWire.write(cmd_mag, sizeof(cmd_mag));  IMUWire.endTransmission();
  }

  if (REQUEST_LAC_REPORTS)
  {
    // request linear acc reports, see 6.5.4
    static const uint8_t cmd_lac[]  = {21, 0, 2, 0, 0xFD, LAC_REPORT_REQUEST_ID,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    IMUWire.beginTransmission(BNO_ADDR);  IMUWire.write(cmd_lac, sizeof(cmd_lac));  IMUWire.endTransmission();
  }

  if (REQUEST_QUAT_REPORTS)
  {
    // request quaternion reports, see 6.5.4
    static const uint8_t cmd_quat[] = {21, 0, 2, 0, 0xFD, QUAT_REPORT_REQUEST_ID, 0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    IMUWire.beginTransmission(BNO_ADDR);  IMUWire.write(cmd_quat, sizeof(cmd_quat));  IMUWire.endTransmission();
  }
  // At 10ms rate, BNO08x outputs most reports in one burst, Gyro-Quat-Lac-Mag, however Acc is asynchronous and a few percent faster. Situation may vary with SENSOR_US and maximum sensor rates.
}

void output_data()
{
  // Convert Quat int16 to floats
  set_fquat(0);
  set_fquat(1);

  // Calculate Euler Angles
  euler_t ypr1_nocal = quaternionToEuler(&(fq[0]), true);
  euler_t ypr2_nocal = quaternionToEuler(&(fq[1]), true);

  // ANGLE DIFFERENCE BETWEEN SENSORS (no calibration)
  fquat_t fq_diff_nocal = differenceQuat(&(fq[0]), &(fq[1]));
  euler_t ypr_diff_nocal = quaternionToEuler(&fq_diff_nocal, true);

  // INCORPORATING CAL QUAT INTO QUAT MEASUREMENTS
  fquat_t fq1_cal = differenceQuat(&(fq[0]), &(fq_ref[0]));
  fquat_t fq2_cal = differenceQuat(&(fq[1]), &(fq_ref[1]));
  euler_t ypr1_cal = quaternionToEuler(&fq1_cal, true);
  euler_t ypr2_cal = quaternionToEuler(&fq2_cal, true);

  // ANGLE DIFFERENCE BETWEEN SENSORS (with calibration)
  fquat_t fq_diff_cal = differenceQuat(&fq1_cal, &fq2_cal);
  euler_t ypr_diff_cal = quaternionToEuler(&fq_diff_cal, true); 

  // Output Data
  for (uint8_t bno = 0; bno < BNOs; bno++)
  {
    // Print data in CSV format
    //Serial.print(bno);            Serial.print(" ");

    // int quat
    //iquat_t *iquat = &(iq[bno]);
    //Serial.print(iquat->qr);       Serial.print("\t");
    //Serial.print(iquat->qi);       Serial.print("\t");
    //Serial.print(iquat->qj);       Serial.print("\t");
    //Serial.print(iquat->qk);       Serial.print("\t");

    // float quat
    //fquat_t *fquat = &(fq[bno]);
    //Serial.print(fquat->qr);       Serial.print("\t");
    //Serial.print(fquat->qi);       Serial.print("\t");
    //Serial.print(fquat->qj);       Serial.print("\t");
    //Serial.print(fquat->qk);       Serial.print("\t");
    //Serial.print("|\t");
  }

  // euler angle (no cal)
  Serial.print("E1_NOCAL: ");
  Serial.print(ypr1_nocal.yaw);      Serial.print(" ");
  Serial.print(ypr1_nocal.pitch);    Serial.print(" ");
  Serial.print(ypr1_nocal.roll);     Serial.print(" ");
  Serial.print("| ");
  Serial.print("E2_NOCAL: ");
  Serial.print(ypr2_nocal.yaw);      Serial.print(" ");
  Serial.print(ypr2_nocal.pitch);    Serial.print(" ");
  Serial.print(ypr2_nocal.roll);     Serial.print(" ");
  Serial.print("| ");


  // differential angle (no cal)
  Serial.print("E_DIFF_NOCAL: ");
  Serial.print(ypr_diff_nocal.yaw);      Serial.print(" ");
  Serial.print(ypr_diff_nocal.pitch);    Serial.print(" ");
  Serial.print(ypr_diff_nocal.roll);     Serial.print(" ");
  Serial.print("| ");

  // euler angle (with cal)
  //Serial.print("E1_CAL: ");
  //Serial.print(ypr1_cal.yaw);      Serial.print(" ");
  //Serial.print(ypr1_cal.pitch);    Serial.print(" ");
  //Serial.print(ypr1_cal.roll);     Serial.print(" ");
  //Serial.print("| ");

  //Serial.print("E2_CAL: ");
  //Serial.print(ypr2_cal.yaw);      Serial.print(" ");
  //Serial.print(ypr2_cal.pitch);    Serial.print(" ");
  //Serial.print(ypr2_cal.roll);     Serial.print(" ");
  //Serial.print("| ");

  //differential euler angle (calibrated)
  Serial.print("E_DIFF_CAL: ");
  Serial.print(ypr_diff_cal.yaw);      Serial.print(" ");
  Serial.print(ypr_diff_cal.pitch);    Serial.print(" ");
  Serial.print(ypr_diff_cal.roll);     
  Serial.println();
}

static void ensure_read_available(uint8_t bno, int16_t length)  // ensure a read byte is available, if necessary reread and discard 4-byte SHTP header, then read as much length as possible
{
  if (!IMUWire.available())
  {
    uint8_t BNO_ADDR = get_bno_addr(bno);
    IMUWire.requestFrom(BNO_ADDR,4+length), IMUWire.read(), IMUWire.read(), IMUWire.read(), IMUWire.read();
  }
}

void check_report(uint8_t bno)
{
  int16_t length;
  uint8_t channel __attribute__((unused));
  uint8_t seqnum  __attribute__((unused));

  uint8_t BNO_ADDR = get_bno_addr(bno);

  IMUWire.requestFrom(BNO_ADDR,4+1);       // read 4-byte SHTP header and first byte of cargo
  if (DEBUG) {Serial.print("SHTP");}
  length  = printbyte(IMUWire.read());     // length LSB
  length |= printbyte((IMUWire.read() & 0x7F) << 8);  // length MSB (ignore continuation flag)
  channel = printbyte(IMUWire.read());     // channel number
  seqnum  = printbyte(IMUWire.read());     // sequence number (ignore)
  length -= 4;                          // done reading SHTP Header
  if (length <= 0 || length > 1000)     // if null/bad/degenerate SHTP header
  {
    if (DEBUG) {Serial.println(" What?");}
    return;
  }
  if (DEBUG) {Serial.print(" L=");  Serial.print(length,HEX);}
  if (DEBUG) {Serial.print(" C=");  Serial.println(channel,HEX);}

  while (length)                        // while more reports in cargo
  {
    uint8_t buf[20];                    // report buffer, big enough for longest interesting report (uninteresting reports will be ignored)
    uint16_t n = 0;                     // index into report buffer

    ensure_read_available(bno, length);
    buf[n++] = printbyte(IMUWire.read());  // first byte of report
    length--;

    // known reports
    if (channel==3 && buf[0]==TIME_REPORT_REQUEST_ID && length >= 5-1)
    {
      for (uint8_t n=1; n<5; n++)       // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(IMUWire.read());
        length--;
      }
      if (DEBUG) {Serial.println(" Time");}
      continue;
    }
    if (channel==3 && buf[0]==ACC_REPORT_REQUEST_ID && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(IMUWire.read());
        length--;
      }
      //iax[bno] = *(int16_t*)&buf[4];
      //iay[bno] = *(int16_t*)&buf[6];
      //iaz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Acc");}
      continue;
    }
    if (channel==3 && buf[0]==GYRO_REPORT_REQUEST_ID && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(IMUWire.read());
        length--;
      }
      //igx[bno] = *(int16_t*)&buf[4];
      //igy[bno] = *(int16_t*)&buf[6];
      //igz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Gyro");}
      continue;
    }
    if (channel==3 && buf[0]==MAG_REPORT_REQUEST_ID && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(IMUWire.read());
        length--;
      }
      //imx[bno] = *(int16_t*)&buf[4];
      //imy[bno] = *(int16_t*)&buf[6];
      //imz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Mag");}
      //output_data(bno);                 // magneto seems to be last report of burst, so use it to trigger data output
      continue;
    }
    if (channel==3 && buf[0]==LAC_REPORT_REQUEST_ID && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(IMUWire.read());
        length--;
      }
      //ilx[bno] = *(int16_t*)&buf[4];
      //ily[bno] = *(int16_t*)&buf[6];
      //ilz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Lac");}
      continue;
    }
    if (channel==3 && buf[0]==QUAT_REPORT_REQUEST_ID && length >= 14-1)
    {
      for (uint8_t n=1; n<14; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(IMUWire.read());
        length--;
      }
      //iqw[bno] = *(int16_t*)&buf[10];
      //iqx[bno] = *(int16_t*)&buf[4];
      //iqy[bno] = *(int16_t*)&buf[6];
      //iqz[bno] = *(int16_t*)&buf[8];
      iquat_t *iquat = &(iq[bno]);
      iquat->qr = *(int16_t*)&buf[10];
      iquat->qi = *(int16_t*)&buf[4];
      iquat->qj = *(int16_t*)&buf[6];
      iquat->qk = *(int16_t*)&buf[8];
      //Serial.print(bno);            Serial.print("\t");
      //Serial.print("int");          Serial.print("\t");
      //Serial.print(iquat->qr);       Serial.print("\t");
      //Serial.print(iquat->qi);       Serial.print("\t");
      //Serial.print(iquat->qj);       Serial.print("\t");
      //Serial.print(iquat->qk);       Serial.println("\t");
      if (DEBUG) {Serial.println(" Quat");}
      continue;
    }

    // unknown reports
    while (length)                      // discard remainder of cargo (shouldn't happen very often)
    {
      ensure_read_available(bno, length);
      printbyte(IMUWire.read());
      length--;
    }
    if (DEBUG) {Serial.println(" Unknown");}
    continue;
  }

  return;
}

euler_t quaternionToEuler(fquat_t *fquat, bool degrees = false)
{
  euler_t angle;

  // get quat components
  float qr = fquat->qr;
  float qi = fquat->qi;
  float qj = fquat->qj;
  float qk = fquat->qk;
  
  // calculate squares of quat components
  float sqr = sq(qr);
  float sqi = sq(qi);
  float sqj = sq(qj);
  float sqk = sq(qk);

  angle.yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
  angle.pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
  angle.roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

  if (degrees) {
    angle.yaw *= RAD_TO_DEG;
    angle.pitch *= RAD_TO_DEG;
    angle.roll *= RAD_TO_DEG;
  }

  return angle;
}

void normalizeQuaternion(fquat_t *fquat)
{
  // get quat components
  float qr = fquat->qr;
  float qi = fquat->qi;
  float qj = fquat->qj;
  float qk = fquat->qk;
  
  // calculate squares of quat components
  float sqr = sq(qr);
  float sqi = sq(qi);
  float sqj = sq(qj);
  float sqk = sq(qk);

  // calculate magnitude
  float quat_mag = sqrt(sqr + sqi + sqj + sqk);

  // divide components by magnitude (normalize)
  float qr_norm;
  float qi_norm;
  float qj_norm;
  float qk_norm;
  if (quat_mag > 0.001)
  {
    qr_norm = qr / quat_mag;
    qi_norm = qi / quat_mag;
    qj_norm = qj / quat_mag;
    qk_norm = qk / quat_mag;
  }

  // modify provided quat
  fquat->qr = qr_norm;
  fquat->qi = qi_norm;
  fquat->qj = qj_norm;
  fquat->qk = qk_norm;
}

fquat_t inverseQuat(fquat_t *fquat)
{
  // create new quat
  fquat_t fquat_inverse;

  // real component stays the same
  fquat_inverse.qr = fquat->qr;
  
  // complex components are inverted
  fquat_inverse.qi = -1 * fquat->qi;
  fquat_inverse.qj = -1 * fquat->qj;
  fquat_inverse.qk = -1 * fquat->qk;

  return fquat_inverse;
}

fquat_t multiplyQuat(fquat_t *fq1, fquat_t *fq2)
{
  // NOT COMMUTATIVE
  // https://paroj.github.io/gltut/Positioning/Tut08%20Quaternions.html
  // create new quat
  fquat_t fq_mult;

  // real component
  fq_mult.qr = fq1->qr * fq2->qr
             - fq1->qi * fq2->qi
             - fq1->qj * fq2->qj
             - fq1->qk * fq2->qk;

  // complex components
  fq_mult.qi = fq1->qr * fq2->qi
             + fq1->qi * fq2->qr
             + fq1->qj * fq2->qk
             - fq1->qk * fq2->qj;

  fq_mult.qj = fq1->qr * fq2->qj
             + fq1->qj * fq2->qr
             + fq1->qk * fq2->qi
             - fq1->qi * fq2->qk;

  fq_mult.qk = fq1->qr * fq2->qk
             + fq1->qk * fq2->qr
             + fq1->qi * fq2->qj
             - fq1->qj * fq2->qi;

  return fq_mult;
}

fquat_t differenceQuat(fquat_t *fq1, fquat_t *fq2)
{
  // calculate inverse of second quat
  fquat_t fq2_inv = inverseQuat(fq2);
  normalizeQuaternion(&fq2_inv);

  // multiply quat 1 and inverse quat 2
  fquat_t fq_diff = multiplyQuat(fq1, &fq2_inv);
  normalizeQuaternion(&fq_diff);

  return fq_diff;
}