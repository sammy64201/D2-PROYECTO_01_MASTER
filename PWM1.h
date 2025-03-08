/*
 * PWM1.h
 *
 * Created: 5/20/2024 9:16:31 PM
 *  Author: samue
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <stdint.h>
#include <avr/io.h>

void init_PWM1(void);
void updateDutyCicle1A(uint16_t duty);
void updateDutyCicle1B(uint16_t duty);



#endif /* PWM1_H_ */