## RoboticArm Libraries

### FingerControl
Manages servo control for robotic fingers:
- Controls 16 PWM servos for finger joints
- Maps finger positions to servo angles
- Implements smooth motion transitions
- Handles servo calibration and limits

### General_ESPNOW
Base ESP-NOW communication protocol implementation:
- Defines data packet structures for position and force data
- Sets up WiFi configuration for ESP-NOW
- Handles message tracking and success rate monitoring
- Configures communication channels and peer devices

### HandControl
Main control logic for the robotic hand:
- Coordinates finger and wrist movement
- Processes incoming position commands
- Manages force sensor readings
- Handles system initialization and setup

### HandControlPanel
Interface control for physical inputs/outputs:
- Manages buttons, switches, and LED indicators
- Controls RGB LED feedback
- Handles button interrupts and state changes
- Provides interface for control panel interactions

### ForceSensors
Force-sensitive resistor interface:
- Reads analog values from FSRs
- Converts voltage readings to force measurements
- Handles sensor calibration
- Manages force data transmission

### RoboticArm
Core system configuration:
- Defines system-wide constants and settings
- Manages FreeRTOS task coordination
- Configures system pins and communication
- Sets up debugging and monitoring options

### RoboticArm_ESPNOW
Arm-specific wireless communication:
- Receives position data from control glove
- Transmits force feedback data
- Manages wireless connection state
- Tracks communication statistics

### SerialServoControl
Feetech serial servo interface for wrist control:
- Manages serial communication with SCS servos
- Implements position and speed control
- Handles servo configuration and status
- Monitors servo current draw

### robotic_arm_pins
Pin definitions and hardware configuration:
- Maps all GPIO pins to functions
- Defines servo, sensor, and interface connections
- Documents hardware interface requirements
- Centralizes pin management 