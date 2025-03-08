/*
 * AHT10.c
 *
 * Created: 3/2/2025 2:07:10 AM
 *  Author: samue
 */ 
#include "AHT10.h"
#include "../I2C/I2C.h"
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t data[6]; // Buffer de datos del AHT10
volatile uint8_t ready_to_read = 0; // Bandera cuando los datos están listos
extern volatile uint8_t data_index; // Se declara como externa porque ya está en main.c

void AHT10_Init(void) {
	_delay_ms(40);

	// Reset del sensor
	I2C_Master_Start();
	I2C_Master_Write(AHT10_ADDRESS << 1);
	I2C_Master_Write(AHT10_CMD_RESET);
	I2C_Master_Stop();
	_delay_ms(20);

	// Calibrar sensor
	I2C_Master_Start();
	I2C_Master_Write(AHT10_ADDRESS << 1);
	I2C_Master_Write(AHT10_CMD_CALIBRATE);
	I2C_Master_Write(0x08);
	I2C_Master_Write(0x00);
	I2C_Master_Stop();
	_delay_ms(500);
}

void AHT10_TriggerMeasurement(void) {
	I2C_Master_Start();
	I2C_Master_Write(AHT10_ADDRESS << 1);
	I2C_Master_Write(AHT10_CMD_MEASURE);
	I2C_Master_Write(0x33);
	I2C_Master_Write(0x00);
	I2C_Master_Stop();
	_delay_ms(500);
}

void AHT10_RequestData(void) {
	data_index = 0;  // Se usa la variable definida en main.c
	ready_to_read = 0;
	
	// Iniciar lectura con interrupciones
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT) | (1 << TWIE);
}

uint8_t AHT10_IsDataReady(void) {
	return ready_to_read;
}

void AHT10_GetRawData(uint32_t *humidity, uint32_t *temperature) {
	if (ready_to_read) {
		ready_to_read = 0;

		*humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
		*temperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
	}
}

// Funciones auxiliares para manejar la interrupción en main.c
void AHT10_StoreReceivedByte(uint8_t receivedByte) {
	data[data_index++] = receivedByte;
}

void AHT10_SetReadyFlag(void) {
	ready_to_read = 1;
}