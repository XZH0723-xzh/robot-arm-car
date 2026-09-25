#ifndef __Bluetooth_H
#define __Bluetooth_H

#include <stdio.h>

extern char Bluetooth_RxPacket[];
extern uint8_t Bluetooth_RxFlag;

void Bluetooth_Init(void);
void Bluetooth_SendByte(uint8_t Byte);
void Bluetooth_SendArray(uint8_t *Array, uint16_t Length);
void Bluetooth_SendString(char *String);
void Bluetooth_SendNumber(uint32_t Number, uint8_t Length);
void Bluetooth_Printf(char *format, ...);

#endif
