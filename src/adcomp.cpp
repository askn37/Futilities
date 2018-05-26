/***************
 *
 * adcomp - A/D compare utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>
#include "adcomp.h"

uint16_t getVcc (void) {
	uint32_t rawVcc = 0;
	#if defined(MUX4)
	// ATmega1284P REFS AVCC - MUX 1.1V
	adcSetup( _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) );
	#else
	// ATmega328P REFS AVCC - MUX 1.1V
	adcSetup( _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) );
	#endif
	for (int n = 0; n < 20; n++){
		rawVcc += adc();
	}
	return 22528000 / rawVcc;		// 1.1 * 2048.0 * 10000
}

void adcSetup (uint8_t data) {
	ADMUX = data;
	ADCSRA |= _BV(ADEN);
	ADCSRA |= 0x07;				// CK/128(Hz)
	delayMicroseconds(1000);
}

uint16_t adc (void){
	ADCSRA |= _BV(ADSC);
	while (bit_is_set(ADCSRA, ADSC));
	uint16_t tADC = ADCW;
	return tADC;
}

// end of code
