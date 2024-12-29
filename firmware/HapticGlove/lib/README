
## HapticGlove Libraries
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

### GloveControl
Main control logic for the haptic glove:
- Coordinates finger tracking, IMU data, and wireless communication
- Manages position data collection and transmission
- Integrates IMU orientation data with finger positions
- Handles system initialization and setup

### GloveControlPanel
Interface control for physical inputs/outputs:
- Manages buttons, switches, and LED indicators
- Controls RGB LED feedback
- Handles button interrupts and state changes
- Provides interface for control panel interactions

### HallEffectSensors
Low-level sensor interface:
- Direct communication with Hall effect sensors
- Converts sensor readings to angle measurements using polynomial fits
- Sensor multiplexing control
- Calibration sequence

### HapticFeedback
High-level haptic control system:
- Manages multiple haptic actuators
- Processes force feedback data
- Controls haptic multiplexing
- Implements feedback patterns for each finger

### HapticGlove
Core system configuration:
- Defines system-wide constants and settings
- Manages FreeRTOS task coordination
- Configures system pins and communication
- Sets up debugging and monitoring options

### HapticGlove_ESPNOW
Glove-specific wireless communication:
- Implements position data transmission
- Handles haptic feedback reception
- Manages wireless connection state
- Tracks communication statistics

### IMU
Inertial Measurement Unit interface:
- Dual BNO085 IMU management
- Quaternion and Euler angle processing
- IMU calibration and orientation tracking
- Sensor fusion and differential measurements

### haptic_glove_pins
Pin definitions and hardware configuration:
- Maps all GPIO pins to functions
- Defines sensor, actuator, and interface connections
- Documents hardware interface requirements
- Centralizes pin management