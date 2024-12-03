#include "HapticFeedback.h"

HapticFeedback::HapticFeedback()
{
    this->sda_pin = HAPTIC_SDA;
    this->scl_pin = HAPTIC_SCL;
    this->mux_addr = 0x70;
}

HapticFeedback::HapticFeedback(uint8_t sda_pin, uint8_t scl_pin, uint8_t mux_addr)
{
    this->sda_pin = sda_pin;
    this->scl_pin = scl_pin;
    this->mux_addr = mux_addr;
}

HapticFeedback::~HapticFeedback() {}

void HapticFeedback::setupFeedback()
{
    i2c_wire.begin(sda_pin, scl_pin, 400000);

    drv.begin(&i2c_wire);

    drv.selectLibrary(6);
    drv.setMode(DRV2605_MODE_INTTRIG);
    drv.useLRA();
}

void HapticFeedback::triggerFeedback()
{
    triggerFeedbackFinger(THUMB);
    triggerFeedbackFinger(INDEX);
    triggerFeedbackFinger(MIDDLE);
    triggerFeedbackFinger(RING);
    triggerFeedbackFinger(PINKY);
}

void HapticFeedback::setHapticMode(uint8_t mode)
{
  drv.setWaveform(0, mode);  // Set effect
  drv.setWaveform(1, 0);     // End waveform
}

void HapticFeedback::triggerFeedbackFinger(Finger finger)
{
    uint8_t force = glove_inData.force_index;
    FingerState *finger_state_ptr;
    switch (finger) {
        case THUMB:
            finger_state_ptr = &this->thumb_state;
            mux_select(THUMB);
        case INDEX:
            finger_state_ptr = &this->index_state;
            mux_select(INDEX);
        case MIDDLE:
            finger_state_ptr = &this->middle_state;
            mux_select(MIDDLE);
        case RING:
            finger_state_ptr = &this->ring_state;
            mux_select(RING);
        case PINKY:
            finger_state_ptr = &this->pinky_state;
            mux_select(PINKY);
    }

    if (force >= LOW_FORCE && force < LOW_MED_FORCE) {
        updateFingerState(LOW_MODE, LOW_MODE_CYCLES, finger_state_ptr);
    } else if (force >= LOW_MED_FORCE && force < MED_FORCE) {
        updateFingerState(LOW_MED_MODE, LOW_MED_MODE_CYCLES, finger_state_ptr);
    } else if (force >= MED_FORCE && force < MED_HIGH_FORCE) {
        updateFingerState(MED_MODE, MED_MODE_CYCLES, finger_state_ptr);
    } else if (force >= MED_HIGH_FORCE && force < HIGH_FORCE) {
        updateFingerState(MED_HIGH_MODE, MED_HIGH_MODE_CYCLES, finger_state_ptr);
    } else if (force >= HIGH_FORCE && force < MAX_FORCE) {
        updateFingerState(HIGH_MODE, HIGH_MODE_CYCLES, finger_state_ptr);
    } else if (force == MAX_FORCE) {
        if (finger_state_ptr->mode != MAX_MODE) {
            setHapticMode(MAX_MODE);
        }
        drv.go();
    }
}

void HapticFeedback::updateFingerState(uint8_t new_mode, uint8_t mode_cycles, FingerState* finger_state_ptr)
{
    if (finger_state_ptr->mode != new_mode) {
        setHapticMode(new_mode);
        drv.go();
        finger_state_ptr->mode = new_mode;
        finger_state_ptr->cycles = 0;
    } else if (finger_state_ptr->cycles >= mode_cycles) {
        drv.go();
        finger_state_ptr->cycles = 0;
    }
}

void HapticFeedback::mux_select(uint8_t channel) {
    if (channel > 7) return;

    i2c_wire.beginTransmission(mux_addr);
    i2c_wire.write(1 << channel);
    i2c_wire.endTransmission();  
}