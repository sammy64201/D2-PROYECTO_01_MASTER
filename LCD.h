/*
 * LCD.h
 *
 * Created: 2/18/2025 4:54:10 PM
 *  Author: samue
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>


void LCD_PORT(char a);
void LCD_CMD(uint8_t cmd);
void LCD_WRT(uint8_t data);
void LCD_STR(char *str);
void LCD_CURSOR(char x, char y);
void init_LCD(void);



#endif /* LCD_H_ */