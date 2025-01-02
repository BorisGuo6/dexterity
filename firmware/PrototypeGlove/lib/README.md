
## PrototypeGlove Libraries
### FingerTracking
Handles finger position tracking using Hall effect sensors:
- Processes angle data from the 16 hall effect sensors to mappings that can be easily converted to servo positions
- Maps sensors to specific finger joints (MCP, PIP, etc.)

### General_ESPNOW
Base ESP-NOW communication protocol implementation:
- Defines data packet structures for position and haptic feedback
- Sets up WiFi configuration for ESP-NOW
- Handles message tracking and success rate monitoring
- Configures communication channels and peer devices

### HallEffectSensors
Low-level sensor interface:
- Direct communication with Hall effect sensors
- Converts sensor readings to angle measurements using polynomial fits
- Sensor multiplexing control
- Calibration sequence

### HapticGlove_ESPNOW
Glove-specific wireless communication:
- Implements position data transmission
- Handles haptic feedback reception
- Manages wireless connection state
- Tracks communication statistics
