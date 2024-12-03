#ifndef _HAPTICFEEDBACK_H_
#define _HAPTICFEEDBACK_H_

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>
#include <haptic_glove_pins.h>
#include <General_ESPNOW.h>

#define LOW_FORCE 1
#define LOW_MED_FORCE 2
#define MED_FORCE 3
#define MED_HIGH_FORCE 4
#define HIGH_FORCE 5
#define MAX_FORCE 6

#define LOW_MODE 6
#define LOW_MED_MODE 23
#define MED_MODE 19
#define MED_HIGH_MODE 18
#define HIGH_MODE 27
#define MAX_MODE 118

#define LOW_MODE_CYCLES 25
#define LOW_MED_MODE_CYCLES 18
#define MED_MODE_CYCLES 13
#define MED_HIGH_MODE_CYCLES 8
#define HIGH_MODE_CYCLES 8

extern haptic_packet glove_inData;

typedef struct {
    uint8_t mode = 0;
    uint8_t cycles = 0;
} FingerState;

enum Finger {
    THUMB,
    INDEX,
    MIDDLE,
    RING,
    PINKY
};

/*
#define HAPTIC_1 6 // low
#define HAPTIC_2 23 // low-medium 
#define HAPTIC_3 19 // medium
#define HAPTIC_4 18 // medium-high
#define HAPTIC_5 27 // high
#define HAPTIC_MAX 118 // max pressure
*/

class HapticFeedback {
    public:
        HapticFeedback();
        HapticFeedback(uint8_t sda_pin, uint8_t scl_pin, uint8_t mux_addr);
        ~HapticFeedback();
        void setupFeedback();
        void triggerFeedback();
    private:
        Adafruit_DRV2605 drv;
        uint8_t sda_pin;
        uint8_t scl_pin;
        TwoWire i2c_wire = TwoWire(0);
        uint8_t mux_addr;
        FingerState thumb_state;
        FingerState index_state;
        FingerState middle_state;
        FingerState ring_state;
        FingerState pinky_state;
        void setHapticMode(uint8_t mode);
        void triggerFeedbackFinger(Finger finger);
        void updateFingerState(uint8_t new_mode, uint8_t mode_cycles, FingerState* finger_state_ptr);
        void mux_select(uint8_t channel);
};

#endif _HAPTICFEEDBACK_H_