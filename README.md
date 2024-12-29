# Project Dexterity

## Overview
Dexterity is a capstone project developed by ECE students at the University of Virginia (Class of 2025). The project consists of a robotic hand that can be controlled using a haptic feedback glove, providing a natural and intuitive way to control robotic movements while receiving tactile feedback.

An summary of the project can be found in these [Figma slides](https://www.figma.com/deck/h1ZrgHnDu9oTQfUKUapgQW/Dexterity-Progress-Report?node-id=32-46&t=BGETurAAA480CFCk-1).

## Project Structure
### Firmware
- `HapticGlove/` - Firmware for the V2 control glove
- `RoboticArm/` - Firmware for the dexhand
- `PrototypeGlove/` - Firmware for the prototype control glove 
- `ESPNOW/` - ESP-NOW wireless communication implementations
- `test/` - Various test implementations and debugging tools

### Hardware
- `cad/` - CAD files for 3D printing
  - `arm base/` - CAD files for the arm base and wire organization
  - `glove v2.4/` - CAD files for the latest interation of the V2 glove
  - `old/` - CAD files for the previous iterations of the V2 glove
- `pcb/`
  - `Forearm/` - PCB design files for the forearm
  - `Hand/` - PCB design files for the hand
  - `Library/` - KiCad library files
  - `gerber/` - Gerber files for manufacturing

### Documentation
- `requirements/` - Project requirements and specifications from our capstone class 
- `reports/` - Project reports and presentations from our capstone class 

### Media
- `images/` - Photos of the project and components
- `videos/` - Demo videos and presentations
- `diagrams/` - System diagrams and illustrations

## Features

### Robotic Arm
- Based on the open-source [Dexhand](https://www.dexhand.org/) project
  - Forearm shell and arm base were FDM printed using PETG filament
  - Everything else was resin printed using ABS-like resin, which provided smoother finishes and more accurate fits due to the higher resolution of SLA printing
- 16 degrees of freedom (DOF) across five fingers
  - 3 DOF per finger (flexion of PIP, flexion of MCP, abduction/adduction)
  - 4 DOF for thumb (extra degree for rotation)
  - Controlled by EMAX ES3352 digital servos
- 2 degrees of freedom in the wrist (pitch and yaw)
  - Controlled by Feetech SCS2332 serial servos 
  - These provide increased range of motion and torque in addition to having overcurrent protection
- Controlled by ESP32-S3 DevKitC-1 microcontroller
  - Housed on a Proto-PCB
  - ESP-NOW protocol for low-latency communication
- Force-sensitive resistors on each fingertip
  - 5kg maximum pressure sensing
  - These are wired up to a voltage divider circuit that connects to ADC channels on the ESP32
- System is powered by a 6V 10A power supply
  - Directly supplies power to servos
  - 3.3V regulation for control electronics
  - Wired through an amp meter for monitoring current draw

### Prototype Glove
- Original repository can be found here: [Prototype Finger Tracker](https://github.com/max-titov/finger-tracker)
- Hall effect sensor-based finger tracking
  - Each joint that was tracked had a rotating magnet and a hall effect sensor
  - 3 DOF per finger (flexion of PIP, flexion of MCP, abduction/adduction)
  - 4 DOF for thumb (extra degree for rotation)
  - Multiplexed sensor reading
- Controlled by a Xiao ESP32-C3 microcontroller
  - ESP-NOW protocol for low-latency communication
- Provided very accurate and responsive finger tracking

### V2 Control Glove
- Same hall effect sensor-based finger tracking as the prototype glove
  - 16 sensors total
  - Electrical complexity of the system introduced a lot of noise into the system, making the finger tracking less accurate than the prototype glove
- Dual PCB design
  - Hand PCB for sensors and haptics
  - Forearm PCB for processing and power
  - Connected with two ribbon cables
- Dual IMU system for wrist orientation
  - One IMU on hand PCB
  - One IMU on forearm PCB
  - Quaternion-based orientation calculation
- Haptic feedback system
  - Linear Resonant Actuators on each fingertip
  - I2C multiplexer for managing multiple LRA drivers
  - Variable intensity based on pressure sensing
- 3D printed design
  - Finger buckets housed LRA drivers and clamped onto fingers using rubber bands
  - Modular finger assemblies allowed easy swapping of parts during development
  - 2 straps per PCB housing attached the device to your hand
- Controlled by ESP32-S3 DevKitC-1 microcontroller
  - Housed on the Forearm PCB
  - ESP-NOW protocol for low-latency communication
- Control panel with LED indicators and mode switches
- Capable of running on battery power due to the onboard 3.3V regulator

## Getting Started
1. Good luck with building everything :)
2. Clone the repository
3. Install PlatformIO extension for VSCode
4. Open the desired project folder
5. Build and upload to your device

## Authors
- Max Titov 
  - Led the team by doing high level design of the whole sytem, assembled the robotic arm, designed the control gloves, and developed finger tracking libraries
  - [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=flat&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/maxim-titov/) [![Twitter](https://img.shields.io/badge/Twitter-1DA1F2?style=flat&logo=twitter&logoColor=white)](https://x.com/MaxTitovvv)
- Alex Schaefer 
  - Architected the embedded software system, implemented ESP-NOW communication, and developed the high-level control structure using multi-threading
  - [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=flat&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/alexschaefer2025/)
- Jackson Lamb 
  - Designed and tested the dual PCB system for the control glove and developed libraries for the control panel interfaces
  - [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=flat&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/jllamb/)
- Jacob Hall 
  - Implemented the dual IMU system for wrist tracking, including quaternion mathematics and calibration routines
  - [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=flat&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/jakethesnakehall/)
- Bhargav Moosani 
  - Designed and implemented the haptic feedback system using LRAs and force sensors, including wireless integration
  - [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=flat&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/bmoosani/)


## Acknowledgments
- [Original Dexhand Repository](https://github.com/TheRobotStudio/V1.0-Dexhand/) - Our robotic hand design is based on The Robot Studio's Dexhand project, licensed under CC BY-NC-SA 4.0.
- [Dexhand Website](https://www.dexhand.org/) - Trent Shumay from IoT Design Shop built two Dexhands and put together a website that compiled info about the Dexhand project into a single page.
- [Prototype Finger Tracker](https://github.com/max-titov/finger-tracker) - My first glove that tracked 16 DOF of the fingers. This glove was used in a lot of the demonstrations of the teleop due to its better finger tracking capabilities.
- [Nepyope's Youtube](https://www.youtube.com/watch?v=iPtgvh6fNdQ) - The video of the first 16 DOF glove using hall effect sensors that inspired this whole project.
- [Nepyope's Project Homunculus](https://github.com/nepyope/Project-Homunculus) - The codebase for the glove that inspired this project.

## License
Shield: [![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg