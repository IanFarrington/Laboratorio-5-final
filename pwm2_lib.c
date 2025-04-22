#include "pwm2_lib.h"

// Configura Timer2 para PWM en OC2B (Pin D3)
void PWM2_init() {
	DDRD |= (1 << PD3); // Pin D3 (OC2B) como salida

	// Modo Phase Correct PWM (TOP = OCR2A)
	TCCR2A = (1 << COM2B1) | (1 << WGM20); // No invertido, modo 5
	TCCR2B = (1 << WGM22) | (1 << CS22) | (1 << CS21);     // Prescaler 256

	OCR2A = 255; // 16 ms (244.14 Hz)
}

void PWM2_set_duty_us(uint16_t microsegundos) {
	OCR2B = (microsegundos);
}