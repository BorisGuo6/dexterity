#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define I2C_SCL_PIN 15
#define I2C_SDA_PIN 16

#define MPU0_I2C_ADDR 0x68
#define MPU1_I2C_ADDR 0x69

TwoWire i2c_wire = TwoWire(0);
Adafruit_MPU6050 mpu_0;
Adafruit_MPU6050 mpu_1;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  i2c_wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, 400000);

  // Try to initialize!
  if (!mpu_0.begin(MPU0_I2C_ADDR, &i2c_wire)) {
    Serial.println("Failed to find MPU 0");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU 0 Found!");

  if (!mpu_1.begin(MPU1_I2C_ADDR, &i2c_wire)) {
    Serial.println("Failed to find MPU 1");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU 1 Found!");

  mpu_0.setAccelerometerRange(MPU6050_RANGE_2_G);
  Serial.print("MPU 0 accelerometer range set to: ");
  switch (mpu_0.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu_0.setGyroRange(MPU6050_RANGE_250_DEG);
  Serial.print("MPU 0 gyro range set to: ");
  switch (mpu_0.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu_1.setAccelerometerRange(MPU6050_RANGE_2_G);
  Serial.print("MPU 1 accelerometer range set to: ");
  switch (mpu_1.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu_1.setGyroRange(MPU6050_RANGE_250_DEG);
  Serial.print("MPU 1 gyro range set to: ");
  switch (mpu_1.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu_0.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("MPU 0 filter bandwidth set to: ");
  switch (mpu_0.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  mpu_1.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("MPU 1 filter bandwidth set to: ");
  switch (mpu_1.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a_0, g_0, temp_0;
  mpu_0.getEvent(&a_0, &g_0, &temp_0);
  sensors_event_t a_1, g_1, temp_1;
  mpu_1.getEvent(&a_1, &g_1, &temp_1);

  /* Print out the values */
  Serial.println("MPU 0");
  Serial.print("Acceleration X: ");
  Serial.print(a_0.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a_0.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a_0.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g_0.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g_0.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g_0.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp_0.temperature);
  Serial.println(" degC");

  Serial.println("MPU 1");
  Serial.print("Acceleration X: ");
  Serial.print(a_1.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a_1.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a_1.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g_1.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g_1.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g_1.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp_1.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
}