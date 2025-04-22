#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "pwm_lib.h"
#include "pwm2_lib.h"

#define MIN_PULSO 1000.0
#define MAX_PULSO 5000.0
#define MIN_PULSO2 7.97
#define MAX_PULSO2 39.84

volatile uint8_t pwm_duty = 0;  // Ciclo de trabajo del LED (0-255)

void ADC_init() {
	ADMUX = (1 << REFS0);         // AVcc como referencia
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}

uint16_t ADC_leer(uint8_t canal) {
	ADMUX = (ADMUX & 0xF0) | (canal & 0x0F); // Selecciona el canal ADC
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void PWM_LED_init() {
	TCCR0A = (1 << WGM01);       // Configura Timer0 en modo CTC (sin salidas PWM)
	TCCR0B = (1 << CS01);        // Prescaler 8 (16MHz/8 = 2MHz ? 0.5µs por tick)
	OCR0A = 39;                  // Interrupción cada 20µs (40 ticks × 0.5µs)
	TIMSK0 |= (1 << OCIE0A);     // Habilita interrupción por comparación
	DDRD |= (1 << DDD4);         // LED en PD4 (D4)
}

int main() {
	PWM_init();     // Inicialización de los servos (Timer1/Timer2)
	PWM2_init();
	ADC_init();
	PWM_LED_init(); // LED con Timer0
	sei();          // Habilita interrupciones

	while(1) {
		uint16_t valor_adc0 = ADC_leer(0);
		uint16_t ancho_pulso1 = (valor_adc0 * ((MAX_PULSO - MIN_PULSO) / 1023.0)) + MIN_PULSO;
		PWM_set_duty_us(ancho_pulso1);
		
		uint16_t valor_adc1 = ADC_leer(1);
		uint16_t ancho_pulso2 = (valor_adc1 * ((MAX_PULSO - MIN_PULSO) / 255.0)) + MIN_PULSO;
		PWM2_set_duty_us(ancho_pulso2);
		
		// LED (tercer potenciómetro en A2)
		uint16_t valor_adc2 = ADC_leer(2);
		pwm_duty = valor_adc2 >> 2; // Escala de 0-1023 a 0-255
	}
}

// ISR para el PWM manual del LED (Timer0)
ISR(TIMER0_COMPA_vect) {
	static uint8_t pwm_counter = 0;
	pwm_counter++;
	if (pwm_counter == 0) PORTD |= (1 << PD4);   // Enciende al inicio del ciclo
	if (pwm_counter == pwm_duty) PORTD &= ~(1 << PD4); // Apaga al alcanzar el duty
}
