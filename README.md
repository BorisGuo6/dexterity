# Project Dexterity

## Overview
Dexterity is a capstone project developed by ECE students at the University of Virginia (Class of 2025). The project consists of a robotic hand that can be controlled using a haptic feedback glove, providing a natural and intuitive way to control robotic movements while receiving tactile feedback.

An general overview of the project can be found in these [Figma slides](https://www.figma.com/deck/h1ZrgHnDu9oTQfUKUapgQW/Dexterity-Progress-Report?node-id=32-46&t=BGETurAAA480CFCk-1).

## Project Structure
### Code
- `HapticGlove/` - Firmware for the V2 control glove
- `RoboticArm/` - Firmware for the robotic hand (mechanical design based on the [Dexhand](https://www.dexhand.org/))
- `PrototypeGlove/` - Firmware for the prototype control glove (mechanical based on [Prototype Finger Tracker](https://github.com/max-titov/finger-tracker))
- `ESPNOW/` - ESP-NOW wireless communication implementations
- `test/` - Various test implementations and debugging tools

### PCB
- `Forearm/` - PCB design files for the forearm
- `Hand/` - PCB design files for the hand
- `Library/` - KiCad library files
- `gerber/` - Gerber files for the PCB

## Features
- Real-time finger position tracking
- Haptic feedback system
- Wireless communication between glove and robotic hand
- 
- 

## Hardware Requirements
- BOM in _____

## Getting Started
1. 
2. Clone the repository
3. Install PlatformIO IDE
4. Open the desired project folder
5. Build and upload to your device

## Documentation
Detailed documentation for each component can be found in their respective folders.

## Authors
- Max Titov
- Alex Schaefer 
- Jackson Lamb
- Jacob Hall
- Bhargav Moosani


## Acknowledgments
- [Original Dexhand Repository](https://github.com/TheRobotStudio/V1.0-Dexhand/) - Our robotic hand design is based on The Robot Studio's Dexhand project, licensed under CC BY-NC-SA 4.0
- [Dexhand Website](https://www.dexhand.org/)
- [Prototype Finger Tracker](https://github.com/max-titov/finger-tracker)
- [Nepyope's Youtube](https://www.youtube.com/watch?v=iPtgvh6fNdQ)
- [Nepyope's Project Homunculus](https://github.com/nepyope/Project-Homunculus)

## License
Shield: [![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg