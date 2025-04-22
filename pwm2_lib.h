#ifndef PWM2_LIB_H
#define PWM2_LIB_H

#include <avr/io.h>

void PWM2_init(void);
void PWM2_set_duty_us(uint16_t microsegundos);

#endif