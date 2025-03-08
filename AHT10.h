/*
 * AHT10.h
 *
 * Created: 3/2/2025 2:06:54 AM
 *  Author: samue
 */ 


#ifndef AHT10_H_
#define AHT10_H_

#include <stdint.h>  

#define AHT10_ADDRESS 0x38
#define AHT10_CMD_MEASURE  0xAC
#define AHT10_CMD_CALIBRATE 0xE1
#define AHT10_CMD_RESET 0xBA

void AHT10_Init(void);
void AHT10_TriggerMeasurement(void);
void AHT10_RequestData(void);
uint8_t AHT10_IsDataReady(void);
void AHT10_GetRawData(uint32_t *humidity, uint32_t *temperature);

// Variables externas para manejar interrupciones en main.c
extern volatile uint8_t data_index;
void AHT10_StoreReceivedByte(uint8_t receivedByte);
void AHT10_SetReadyFlag(void);




#endif /* AHT10_H_ */