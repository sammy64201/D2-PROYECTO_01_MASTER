/*
 * UART.c
 *
 * Created: 2/11/2025 3:15:20 PM
 *  Author: samue
 */ 
#include "UART.h"


void writeUART(char Caracter){
	// si no es 1 quedate esperando POR LO TANTO HASTA QUE SEA 1 VA CONTINUAR
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
}

void WriteTextUART(char * Texto){
uint8_t i = 0;
while (Texto[i] != '\0'){ // Mientras no sea el final de la cadena
	writeUART(Texto[i]);  // Usar la función writeUART() para enviar cada carácter
	i++;
}
}

void initUART9600(){
	// paso 1: establecer pines como entrada y salida
	//RX COMO ENTRADA Y TX COMO SALIDA
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	// PASO 2: configurar UCSR0A
	UCSR0A = 0;
	
	// PASO 3: configurar UCSR0B  DONDE SE HABILITA LA ISR DE RECEPCION
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);

	//PASO 4: CONFIGURAR UCSR0C
	UCSR0C = 0;
	//CONFIGURAR ASINCORONO Y SIN PARIDAD, 1 BIT DE STOP, 8 CARACTERES
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	//PASO 5: SE CALCULA EL VALOR DE BAUD Y VELOCIDAD: 9600
	UBRR0 = 103;
}

uint8_t recieveUART(void){
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}