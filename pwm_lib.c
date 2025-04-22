#include "pwm_lib.h"

// Configura Timer1 para PWM en OC1A (Pin 9) con periodo de 20ms (50Hz)
void PWM_init() {
	DDRB |= (1 << PB1); // Pin D9 como salida

	// Modo Fast PWM (ICR1 como TOP, no invertido)
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler 8

	ICR1 = 39999; // Frecuencia = 50Hz (Período 20ms)
}

void PWM_set_duty_us(uint16_t microsegundos) {
	OCR1A = (microsegundos); // 1 tick = 0.5µs (16MHz/8)
}