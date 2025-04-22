#ifndef PWM_LIB_H
#define PWM_LIB_H

#include <avr/io.h>

void PWM_init(void);
void PWM_set_duty_us(uint16_t microsegundos);

#endif