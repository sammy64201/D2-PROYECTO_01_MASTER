/*
 * LCD.c
 *
 * Created: 2/18/2025 4:54:32 PM
 *  Author: samue
 */ 
#include "LCD.h"



void LCD_PORT(char a){
	// PINES DE LA PANTALLA
	// D0-D5
	PORTD = (PORTD & 0x03) | ((a & 0x3F) << 2);
	// D6-D7
	PORTB = (PORTB & 0xFC) | ((a >> 6) & 0x03);
	_delay_ms(10);
}
void LCD_CMD(uint8_t cmd){
	
	PORTB &= ~(1 << PB2);
	LCD_PORT(cmd);
	
	// EN = 1 --> E = 1
	PORTB |= (1 << PB3);
	_delay_ms(6);
	
	// EN = 0 --> E = 0
	PORTB &= ~(1 << PB3);
}
void LCD_WRT(uint8_t data){
	// RS = 1
	PORTB |= (1 << PB2);
	LCD_PORT(data);	
	// EN = 1 --> E = 1
	PORTB |= (1 << PB3);
	_delay_ms(4);
	// EN = 0 --> E = 0
	PORTB &= ~(1 << PB3);
}
void LCD_STR(char *str){
	while (*str) {
		LCD_WRT(*str);
		str++;
	}
}
void LCD_CURSOR(char x, char y){
	char pos = (y == 0) ? (0x80 + x) : (0xC0 + x);
	LCD_CMD(pos);
}
void init_LCD(){
	/* 
	D0 --> PD2
	D1 --> PD3
	D2 --> PD4
	D3 --> PD5
	D4 --> PD6
	D5 --> PD7
	D6 --> PB0
	D7 --> PB1
	RS --> PB2
	E  --> PB3
	*/
	// DECLARACION DE PINES DE SALIDA DE PANTALLA 
	DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
	_delay_ms(50);
	
	// MODO 8 BITS
	LCD_CMD(0x38);	// 0011 1000
	_delay_ms(5);	
	// DISPLAY ON 
	LCD_CMD(0x0C);	// 0000 1100 
	_delay_ms(5);
	// ENTRY MODE 
	LCD_CMD(0x06);	// 0000 0110
	_delay_ms(5);
	// CLEAR DISPLAY
	LCD_CMD(0x01);	// 0000 0001
	_delay_ms(5);
	
	
}

	