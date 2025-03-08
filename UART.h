/*
 * UART.h
 *
 * Created: 2/11/2025 3:15:04 PM
 *  Author: samue
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

void initUART9600(void);			// FUNCION PARA INICIAR UART
void writeUART(char Caracter);		// FUNCION PARA MANDAR CARACTER POR CARACTER
void WriteTextUART(char * Texto);	// FUNCION PARA ENVIAR UN TEXTO COMPLETO
uint8_t recieveUART(void);





#endif /* UART_H_ */