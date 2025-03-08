/*
 * I2C.c
 *
 * Created: 2/18/2025 4:45:10 PM
 *  Author: samue
 */ 
#include "I2C.h"


//Funci�n para inicializar I2C Maestro
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler){
	DDRC &= ~((1<<DDC4))|(1<<DDC5);  // Pines de I2C como entradas
	
	switch (Prescaler){
		case 1:
			TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		break;
		case 4:
			TWSR &= ~(1<<TWPS1);
			TWSR |= (1<<TWPS0);
		break;
		case 16:
			TWSR &= ~(1<<TWPS0);
			TWSR |= (1<<TWPS1);
		break;
		case 64:
			TWSR |= (1<<TWPS1) | (1<<TWPS0);
		break;
		default:
			TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
			Prescaler = 1;
		break;
	}
	TWBR = ((F_CPU/SCL_Clock))-16/(2*Prescaler);
	TWCR |= (1<<TWEN);
}

//Funci�n de inicio de la comunicaci�n I2C
void I2C_Master_Start(void){
	//uint8_t estado;
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); //Iniciar condicion de start
	while(!(TWCR & (1<< TWINT))); // Espera a que termine la flag TWINT
	
	/*estado = TWSR & 0xF8; //Verificar estado
	if ((estado != 0x08) || (estado != 0x10){
		return 1;
	}else{
		return estado;
	}*/
}

//Funci�n de parada de la comunicaci�n I2C
void I2C_Master_Stop(void){
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTO); //Inicia el envio secuencia parada STOP
	//while(TWCR & (1<< TWSTO));
}

//Funci�n de transmisi�n de datos del maestro al esclavo 
uint8_t I2C_Master_Write(uint8_t dato){
	uint8_t estado;
	
	TWDR = dato; //Cargar el dato
	TWCR = (1<<TWEN)|(1<<TWINT); //Inicia el env�o
	
	while(!(TWCR & (1<< TWINT))); //Esperar al flag TWINT
	estado = TWSR & 0xF8; //Verificar estado
	// Verificar si se tranmitio una SLA + W con ACK, SLA + R con ACK, o un Daato con ACK
	if(estado == 0x18 || estado == 0x28 || estado == 0x40){
		return 1;
	}else{
		return estado;
	}
}

//Funci�n de recepci�n de datos enviados por el esclavo al maestro
//esta funcion es para leer los datos que estan en el esclavo
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	if(ack){
		TWCR != (1<<TWEA);
	}else{
		TWCR &= ~(1<<TWEA);
	}
	
	TWCR != (1<<TWINT); // Iniciamos la lectura
	while(!(TWCR & (1<< TWINT))); // Espera al flag TWINT
	estado = TWSR & 0xF8;
	//Verificar dato le�do con ACK o sin ACK
	if(estado == 0x58 || estado == 0x50){
		*buffer = TWDR;
		return 1;
	}else{
		return estado;
	}
}

// Funci�n para inicializar I2C Esclavo
void I2C_Slave_Init(uint8_t address){
	DDRC &= ~((1<<DDC4)|(1<<DDC5)); // Pines de I2C como entradas
	
	TWAR = address << 1; // Se asigna la direcci�n que tendra
	//TWAR = (address << 1| 0x01); //Se asigna la direcci�n	que tendr� y habilita llamada
	
	//Se habilita la interfaz, ACK automatico, se habliita la ISR
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}