/*
 * PWM1.c
 *
 * Created: 5/20/2024 9:16:55 PM
 *  Author: samue
 */ 
#include "PWM1.h"
void init_PWM1(void){
	// DECLARACION DE PUERTOS DE SALIDA 
	DDRB |= (1 << DDB1) | (1 << DDB2);
	TCCR1A = 0;
	TCCR1B = 0;
	// USAMOS NO INVERTIDO 
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << COM1B1);
	
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13);
	
	// SE USA PRESCALER DE 64
	TCCR1B |= (1<<CS11)|(1<<CS10);
	
	//TOP 1249
	ICR1H = 0x04;
	ICR1L = 0xE1;
}

void updateDutyCicle1A(uint16_t duty){
	OCR1A = duty;
}

void updateDutyCicle1B(uint16_t duty){
	OCR1B = duty;
}