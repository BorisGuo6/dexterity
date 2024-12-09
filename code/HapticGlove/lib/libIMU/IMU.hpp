#ifndef IMU_HPP
#define IMU_HPP

// Adafruit BNO085 Board: https://www.adafruit.com/product/4754
// BNO085 Chip Datasheet: https://www.ceva-ip.com/wp-content/uploads/2019/10/BNO080_085-Datasheet.pdf

#include <Arduino.h>
#include <Wire.h>

// _____________  COMPILATION SETTINGS ____________________
#define DEBUG       0           // output extra info: 0 off, 1 on. Beware this causes too much output data at low baud rates and/or high sensor rates.
#define TELEPLOT    0           // output data in Teleplot format: 0 off, 1 on.
#define REQUEST_ACC_REPORTS     0      // request accelerometer data from IMU
#define REQUEST_GYRO_REPORTS    0      // request gyroscope data from IMU
#define REQUEST_MAG_REPORTS     1      // request magnetometer data from IMU (THIS NEEDS TO BE 1 FOR QUAT OUTPUT FOR SOME REASON)
#define REQUEST_LAC_REPORTS     0      // request linear acceleration data from IMU
#define REQUEST_QUAT_REPORTS    1      // request quaternion data from IMU (THIS IS WHAT WE USE TO CALC EULER ANGLES)
#define REQUEST_TIME_REPORTS    0      // request time data from IMU

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

// BREADBOARD SETUP
//#define pinRST      36            // output pin to BNO RST
//#define IMU_I2C_SDA     35              // I2C SDA pin
//#define IMU_I2C_SCL     0               // I2C SCL pin
//#define IMU_CAL_BUTTON    5

// PCB SETUP
#define pinRST      38            // output pin to BNO RST
#define IMU_I2C_SDA     37              // I2C SDA pin
#define IMU_I2C_SCL     36               // I2C SCL pin
#define IMU_CAL_BUTTON    11
#define BUTTON_NORMAL   HIGH

// _____________ I2C SETTINGS ____________________
#define TWOWIRE_INSTANCE_ADDRESS 1 // used to change the TwoIMUWire address (not I2C address)
#define BNOs        2
#define BNO_ADDR1   0x4A          // I2C address of 1st BNO085 sensor (0x4A if SA0=0, 0x4B if SA0=1)
#define BNO_ADDR2   0x4B          // I2C address of 2nd '''
#define I2C_CLOCK   400000L       // I2C clock rate
#define SERIAL_BAUD 9600L     // serial port baud rate
#define SENSOR_US   20000L      // time between sensor reports, microseconds, 10000L is 100 Hz, 20000L is 50 Hz

// _____________ IMU REQUEST IDS ____________________ (DO NOT CHANGE)
#define ACC_REPORT_REQUEST_ID   0x01   // accel report, see 6.5.9
#define GYRO_REPORT_REQUEST_ID  0x02   // gyro report, see 6.5.13
#define MAG_REPORT_REQUEST_ID   0x03   // magneto report, see 6.5.16
#define LAC_REPORT_REQUEST_ID   0x04   // linear accel report, see 6.5.10
#define QUAT_REPORT_REQUEST_ID  0x05   // quaternion report, see 6.5.18
#define TIME_REPORT_REQUEST_ID  0xFB   // time report, see 7.2.1

// _____________ OTHER MACROS ____________________
#define SCALE_Q(n) (1.0f / (1 << n))

// _____________ TYPE DEFINITIONS ____________________
struct iquat_t {
    int16_t qr;
    int16_t qi;
    int16_t qj;
    int16_t qk;
};

struct fquat_t {
    float qr;
    float qi;
    float qj;
    float qk;
};

struct euler_t {
  float yaw;
  float pitch;
  float roll;
};

// _____________ FUNCTION HEADERS ____________________
void initializeIMUs();
void calibrateIMUs();
bool isIntQuatZero(uint8_t);
euler_t getAngleDifference();
void set_fquat(uint8_t);
void set_cal_quat(uint8_t);
uint8_t get_bno_addr(uint8_t);
static void request_reports(uint8_t);
void output_data();
static void ensure_read_available(uint8_t, int16_t);
void check_report(uint8_t);
euler_t process_quats(uint8_t);
euler_t quaternionToEuler(fquat_t *, bool);
void normalizeQuaternion(fquat_t *);
fquat_t inverseQuat(fquat_t *);
fquat_t multiplyQuat(fquat_t *, fquat_t *);
fquat_t differenceQuat(fquat_t *, fquat_t *);

// *******************
// **  Output data  **
// *******************
extern iquat_t iq[];           // int quaternion, reported by sensor                                                              
extern fquat_t fq[];           // float quaternion, converted from int quat
extern fquat_t fq_ref[];       // reference quaternions for calibration

extern int imu_cal_button_state;

// I2C BUS
extern TwoWire IMUWire;

#endif