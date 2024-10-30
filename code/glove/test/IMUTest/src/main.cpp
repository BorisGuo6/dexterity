// Adafruit BNO085 Board: https://www.adafruit.com/product/4754
// BNO085 Chip Datasheet: https://www.ceva-ip.com/wp-content/uploads/2019/10/BNO080_085-Datasheet.pdf

#include <Arduino.h>
#include <Wire.h>

// _____________  COMPILATION SETTINGS ____________________
#define DEBUG       0           // output extra info: 0 off, 1 on. Beware this causes too much output data at low baud rates and/or high sensor rates.
#define TELEPLOT    0           // output data in Teleplot format: 0 off, 1 on.

#if DEBUG
  static uint8_t printbyte(uint8_t b)
  {
    Serial.print(" ");  Serial.print(b,HEX);
    return b;
  }
#else
  #define printbyte(b) (b)
#endif

// _____________ PIN DEFINITIONS ____________________
#define pinRST      47            // output pin to BNO RST
#define I2C_SDA     35              // I2C SDA pin
#define I2C_SCL     0               // I2C SCL pin

// _____________ I2C SETTINGS ____________________
#define BNOs        2
#define BNO_ADDR1   0x4A          // I2C address of 1st BNO085 sensor (0x4A if SA0=0, 0x4B if SA0=1)
#define BNO_ADDR2   0x4B          // I2C address of 2nd '''
#define I2C_CLOCK   400000L       // I2C clock rate
#define SERIAL_BAUD 9600L     // serial port baud rate
#define SENSOR_US   20000L      // time between sensor reports, microseconds, 10000L is 100 Hz, 20000L is 50 Hz

// _____________ OTHER MACROS ____________________
#define SCALE_Q(n) (1.0f / (1 << n))

// _____________ TYPE DEFINITIONS ____________________
struct euler_t {
  float yaw;
  float pitch;
  float roll;
};

// _____________ FUNCTION HEADERS ____________________
static void request_reports(uint8_t);
void uart_b64(int32_t);
static void output_data(uint8_t);
static void ensure_read_available(uint8_t, int16_t);
static void check_report(uint8_t);
void quaternionToEuler(float, float, float, float, euler_t*, bool);

// *******************************
// **  Request desired reports  **
// *******************************
#define ACC_REPORT   0x01   // accel report, see 6.5.9
#define GYRO_REPORT  0x02   // gyro report, see 6.5.13
#define MAG_REPORT   0x03   // magneto report, see 6.5.16
#define LAC_REPORT   0x04   // linear accel report, see 6.5.10
#define QUAT_REPORT  0x05   // quaternion report, see 6.5.18
#define TIME_REPORT  0xFB   // time report, see 7.2.1

// *******************
// **  Output data  **
// *******************
int16_t iax[BNOs], iay[BNOs], iaz[BNOs];             // accel, integer
int16_t igx[BNOs], igy[BNOs], igz[BNOs];             // gyro, integer
int16_t imx[BNOs], imy[BNOs], imz[BNOs];             // magneto, integer
int16_t ilx[BNOs], ily[BNOs], ilz[BNOs];             // linear accel, integer
int16_t iqw[BNOs], iqx[BNOs], iqy[BNOs], iqz[BNOs];  // quaternion, integer
float   fqw[BNOs], fqx[BNOs], fqy[BNOs], fqz[BNOs];  // quaternion, float
euler_t ypr[BNOs];                                   // euler angles, float

char obuf[70], *pbuf;           // ensure this output buffer is big enough for your output string!



// _____________ SETUP ____________________
void setup()
{
  // initialize serial
  Serial.begin(SERIAL_BAUD);            
  Serial.println("\nRunning...");

  // reset all BNOs
  pinMode(pinRST,OUTPUT);               
  digitalWrite(pinRST,LOW);
  delay(1);
  digitalWrite(pinRST,HIGH);
  delay(300);

  // initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);                         
  Wire.setClock(I2C_CLOCK);

  // request desired reports
  for (uint8_t bno=0; bno<BNOs; bno++)  
  {
    Serial.print("requesting desired reports for ");
    Serial.println(bno);
    request_reports(bno);
  }
}


// _____________ LOOP ____________________
void loop()
{
  for (uint8_t bno=0; bno<BNOs; bno++)  // check for reports from all BNOs
  {
    check_report(bno);
    
  }
      
}


// _____________ FUNCTION DEFINITIONS _________________

static void request_reports(uint8_t bno)
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

  // request acc reports, see 6.5.4
  static const uint8_t cmd_acc[]  = {21, 0, 2, 0, 0xFD, ACC_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_acc, sizeof(cmd_acc));  Wire.endTransmission();

  // request gyro reports, see 6.5.4
  static const uint8_t cmd_gyro[] = {21, 0, 2, 0, 0xFD, GYRO_REPORT, 0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_gyro, sizeof(cmd_gyro));  Wire.endTransmission();

  // request magneto reports, see 6.5.4
  static const uint8_t cmd_mag[]  = {21, 0, 2, 0, 0xFD, MAG_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_mag, sizeof(cmd_mag));  Wire.endTransmission();

  // request linear acc reports, see 6.5.4
  static const uint8_t cmd_lac[]  = {21, 0, 2, 0, 0xFD, LAC_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_lac, sizeof(cmd_lac));  Wire.endTransmission();

  // request quaternion reports, see 6.5.4
  static const uint8_t cmd_quat[] = {21, 0, 2, 0, 0xFD, QUAT_REPORT, 0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
  Wire.beginTransmission(BNO_ADDR);  Wire.write(cmd_quat, sizeof(cmd_quat));  Wire.endTransmission();

  // At 10ms rate, BNO08x outputs most reports in one burst, Gyro-Quat-Lac-Mag, however Acc is asynchronous and a few percent faster. Situation may vary with SENSOR_US and maximum sensor rates.
}


void uart_b64(int32_t i)        // output 18-bit integer as compact 3-digit base64
{
  for (int n=12; n >= 0; n-=6)
  {
    uint8_t c = (i >> n) & 63;
    *pbuf++ = (char)(c<26 ? 'A'+c : c<52 ? 'a'-26+c : c<62 ? '0'-52+c : c==62 ? '+' : '/');
  }
}


static void output_data(uint8_t bno)
{
  // Convert Quat int16 to floats
  fqw[bno] = iqw[bno] * SCALE_Q(8);
  fqx[bno] = iqx[bno] * SCALE_Q(8);
  fqy[bno] = iqy[bno] * SCALE_Q(8);
  fqz[bno] = iqz[bno] * SCALE_Q(8);

  // Calculate Euler Angles
  quaternionToEuler(fqw[bno], fqx[bno], fqy[bno], fqz[bno], &ypr[bno], true);

  // Output Data
  if (TELEPLOT)
  {
    // Print Data in Teleplot format if compiler flag is set to 1
    Serial.print(">yaw");
    Serial.print(bno);
    Serial.print(":");
    Serial.println(ypr[bno].yaw);

    Serial.print(">pitch");
    Serial.print(bno);
    Serial.print(":");
    Serial.println(ypr[bno].pitch);

    Serial.print(">roll");
    Serial.print(bno);
    Serial.print(":");
    Serial.println(ypr[bno].roll);
  }
  else
  {
    // Print data in CSV format
    Serial.print(bno);            Serial.print("\t");
    Serial.print(ypr[bno].yaw);   Serial.print("\t");
    Serial.print(ypr[bno].pitch); Serial.print("\t");
    Serial.println(ypr[bno].roll);
  }
}


static void ensure_read_available(uint8_t bno, int16_t length)  // ensure a read byte is available, if necessary reread and discard 4-byte SHTP header, then read as much length as possible
{
  if (!Wire.available())
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
    Wire.requestFrom(BNO_ADDR,4+length), Wire.read(), Wire.read(), Wire.read(), Wire.read();
  }
}


static void check_report(uint8_t bno)
{
  int16_t length;
  uint8_t channel __attribute__((unused));
  uint8_t seqnum  __attribute__((unused));

  uint8_t BNO_ADDR;
  if (bno == 1) 
  {
    BNO_ADDR = BNO_ADDR2;
  }
  else 
  {
    BNO_ADDR = BNO_ADDR1;
  }

  Wire.requestFrom(BNO_ADDR,4+1);       // read 4-byte SHTP header and first byte of cargo
  if (DEBUG) {Serial.print("SHTP");}
  length  = printbyte(Wire.read());     // length LSB
  length |= printbyte((Wire.read() & 0x7F) << 8);  // length MSB (ignore continuation flag)
  channel = printbyte(Wire.read());     // channel number
  seqnum  = printbyte(Wire.read());     // sequence number (ignore)
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
    buf[n++] = printbyte(Wire.read());  // first byte of report
    length--;

    // known reports
    if (channel==3 && buf[0]==TIME_REPORT && length >= 5-1)
    {
      for (uint8_t n=1; n<5; n++)       // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(Wire.read());
        length--;
      }
      if (DEBUG) {Serial.println(" Time");}
      continue;
    }
    if (channel==3 && buf[0]==ACC_REPORT && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(Wire.read());
        length--;
      }
      iax[bno] = *(int16_t*)&buf[4];
      iay[bno] = *(int16_t*)&buf[6];
      iaz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Acc");}
      continue;
    }
    if (channel==3 && buf[0]==GYRO_REPORT && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(Wire.read());
        length--;
      }
      igx[bno] = *(int16_t*)&buf[4];
      igy[bno] = *(int16_t*)&buf[6];
      igz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Gyro");}
      continue;
    }
    if (channel==3 && buf[0]==MAG_REPORT && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(Wire.read());
        length--;
      }
      imx[bno] = *(int16_t*)&buf[4];
      imy[bno] = *(int16_t*)&buf[6];
      imz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Mag");}
      output_data(bno);                 // magneto seems to be last report of burst, so use it to trigger data output
      continue;
    }
    if (channel==3 && buf[0]==LAC_REPORT && length >= 10-1)
    {
      for (uint8_t n=1; n<10; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(Wire.read());
        length--;
      }
      ilx[bno] = *(int16_t*)&buf[4];
      ily[bno] = *(int16_t*)&buf[6];
      ilz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Lac");}
      continue;
    }
    if (channel==3 && buf[0]==QUAT_REPORT && length >= 14-1)
    {
      for (uint8_t n=1; n<14; n++)      // read remainder of report
      {
        ensure_read_available(bno, length);
        buf[n] = printbyte(Wire.read());
        length--;
      }
      iqw[bno] = *(int16_t*)&buf[10];
      iqx[bno] = *(int16_t*)&buf[4];
      iqy[bno] = *(int16_t*)&buf[6];
      iqz[bno] = *(int16_t*)&buf[8];
      if (DEBUG) {Serial.println(" Quat");}
      continue;
    }

    // unknown reports
    while (length)                      // discard remainder of cargo (shouldn't happen very often)
    {
      ensure_read_available(bno, length);
      printbyte(Wire.read());
      length--;
    }
    if (DEBUG) {Serial.println(" Unknown");}
    continue;
  }
  return;
}


void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {

    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
      ypr->yaw *= RAD_TO_DEG;
      ypr->pitch *= RAD_TO_DEG;
      ypr->roll *= RAD_TO_DEG;
    }
}