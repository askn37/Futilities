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

#if defined(REFS0)
#include <Arduino.h>
#include "adcomp.h"

uint16_t getVcc (void) {
    uint32_t rawVcc = 0;
    #if defined(MUX4)
    // ATmega1284P REFS AVCC - MUX 1.1V
    adcSetup( _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) );
    #else
    // ATmega32U4
    // ATmega328P REFS AVCC - MUX 1.1V
    adcSetup( _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) );
    #endif
    for (int n = 0; n < 20; n++){
        rawVcc += adc();
    }
    return 22528000 / rawVcc;		// 1.1 * 1024.0 * 20.0 * 1000.0
}

void adcSetup (uint8_t data) {
    ADMUX = data;
    ADCSRA |= _BV(ADEN);
    ADCSRA |= 0x07;				    // CK/128(Hz)
    delayMicroseconds(1000);
}

uint16_t adc (void){
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC));
    uint16_t tADC = ADCW;
    return tADC;
}

void openDrain (uint8_t pin, bool state) {
    volatile uint8_t *reg = portModeRegister(digitalPinToPort(pin));
    uint8_t bit = digitalPinToBitMask(pin);
    if (state) *reg &= ~bit;
    else       *reg |= bit;
}

float getThermistor (uint8_t pin, float Tb, float Ta, float Tr, float r1) {
    int Vt = analogRead(pin);
    float Rt = r1 * (1023.0 / (float)Vt - 1.0);
    float Tp = (1 / ( (log(Rt / Tr) ) / Tb + 1 / (273.15 + Ta) )) - 273.15;
    return Tp;
}

#ifdef __ADCOMP_ENABLE_ACP
volatile void __acp_vect_empty (void) {}
volatile void (*__acp_vect)(void);
// ISR(ANALOG_COMP_vect, __attribute__ ((weak))) { __acp_vect(); }
ISR(ANALOG_COMP_vect) { __acp_vect(); }

void acpSetup (void) {
    // ADSCRB : ADC Control and Status Register B
    //   ACME : Analog Comparator Multiplexer Enable OFF=0
    ADCSRB &= ~_BV(ACME);

    // ADCSRA : ADC Control and Status Register A
    //   ADEN : ADC Enable 0:Power Off
    ADCSRA &= ~_BV(ADEN);

    // ACSR : Analog Comparator Control and Status Register
    // ACD  : Analog Comparator Disable ON=0
    // ACIC : Analog Comparator Input Capture Enable OFF=0
    // ACISn : Analog Comparator Interrupt Mode Select
    //    00 : Toggle
    //    10 : Edge FALSE
    //    11 : Edge RAISE
    ACSR &= ~( _BV(ACD) | _BV(ACIC) | _BV(ACIS1) | _BV(ACIS0) );
    // ACBG : Analog Comparator Bandgap Select
    //   ON : internal 1.1V
    //  OFF : AIN0 Pin sensing
    // ACIE : Analog Comparator Interrupt Enable
    ACSR |= _BV(ACBG) | _BV(ACIE);

    // PRR : Power Reduction Register
    // PRADC : Power Reduction ADC
    // PRR |= _BV(PRADC);
}

void acpAttachInterrupt (uint8_t pin, void(*callback)(void), int mode) {
    uint8_t acsr = (uint8_t)mode & 3;	// CHANGE FALLING RISING
    if (acsr == 1) acsr = 0;
    if (pin < 8) {
        acsr |= pin;
        ADCSRB |= _BV(ACME);
        ADCSRA &= ~_BV(ADEN);
    }
    else {
        ADCSRA |= _BV(ADEN);
        ADCSRB &= ~_BV(ACME);
        if (pin == 8) acsr |= _BV(ACBG);
    }

    if (callback == NULL) {
        // Interrupt Disable
        ACSR &= ~_BV(ACIE);
        __acp_vect = (volatile void (*)(void)) __acp_vect_empty;
    }
    else {
        // Interrupt Enable
        __acp_vect = (volatile void (*)(void)) callback;
        acsr |= _BV(ACIE);
        ACSR = acsr;
    }
}
#endif
#endif

// end of code
