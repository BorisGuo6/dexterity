#ifndef ROBOTIC_ARM_ESPNOW_H
#define ROBOTIC_ARM_ESPNOW_H

#include <General_ESPNOW.h>

// Start RoboticArm_ESPNOW.h:

// Create an instance of the struct to be sent/received
// Create it in public space so general arm code can access values
extern position_packet arm_inData;
extern haptic_packet arm_outData;

// remove when not monitoring success rate:
extern int arm_messages_send_attempt;
extern int arm_messages_send_success;
extern int arm_messages_rcv;

// general arm code needs to initialize ESPNOW
void arm_ESPNOWsetup(uint8_t mac_in[], int baud_rate); // starts UART0
void arm_ESPNOWsetup(uint8_t mac_in[]); // UART0 already started

// general arm code has access to sendData function
void arm_sendData(uint8_t fi, uint8_t fm, uint8_t fr, uint8_t fp, uint8_t ft);

// receive data function will call general arm code

void arm_monitorSuccess();

// End RoboticArm_ESPNOW.h
#endif