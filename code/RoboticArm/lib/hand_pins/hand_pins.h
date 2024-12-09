#ifndef _HAND_PINS_H_
#define _HAND_PINS_H_

// file to define pins on ESP32 board

// left side pins
#define PRESSURE_SENSOR_0 4     // 4
#define PRESSURE_SENSOR_1 5     // 5
#define PRESSURE_SENSOR_2 6     // 6
#define PRESSURE_SENSOR_3 7     // 7
#define DIG_SERVO_SDA 1        // 1
#define DIG_SERVO_SCL 2        // 2
#define SER_SERVO_TX 17         // 17
#define SER_SERVO_RX 18         // 18
#define PRESSURE_SENSOR_4 8     // 8
// pin 3 unused                 // 3
// pin 46 unused                // 46
#define SWITCH_0 9              // 9
#define SWITCH_1 10             // 10
#define BUTTON_0 37             // 37
#define BUTTON_1 12             // 12
#define LED_0 36                // 36
#define LED_1 35                // 35

// right side pins
#define U0_TX 43                // TX
#define U0_RX 44                // RX
#define IMU_INT_0 1             // 1
#define IMU_INT_1 2             // 2
// pin 42 unused                // 42
// pin 41 unused                // 41
// pin 40 unused                // 40
// pin 39 unused                // 39
#define IMU_RST 38              // 38
#define IMU_SDA 37              // 37
#define IMU_SCL 36              // 36
// pin 35 unused                // 35
// pin 0 unused                 // 0
// pin 45 unused                // 45
#define INTERNAL_RGB_LED 48     // 48
// pin 47 unused                // 47
// pin 21 unused                // 21
#define USB_D_PLUS 20           // 20
#define USB_D_MINUS 19          // 19

#endif