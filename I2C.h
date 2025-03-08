/*
 * I2C.h
 *
 * Created: 2/18/2025 4:44:56 PM
 *  Author: samue
 */ 

#define F_CPU 16000000UL
#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <stdint.h>

//Funcion para inicializar I2C Maestro
void I2C_Master_Init (unsigned long SCL_Clock, uint8_t Prescaler);

//Función de inicio de la comunicación I2C
void I2C_Master_Start(void);

//Función de parada de la comunicación I2C
void I2C_Master_Stop(void);

//Función de transmisión de datos del maestro al esclavo
uint8_t I2C_Master_Write(uint8_t dato);

//Función de recepción de datos enviados por el esclavo al maestro
//Esta función es para leer los datos que estan en el esclavo
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack);

//Función para inicializar I2C Esclavo
void I2C_Slave_Init(uint8_t address);




#endif /* I2C_H_ */