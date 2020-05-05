/***************
 *
 * chore - chore macro utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
#ifndef __CHORE_H
#define __CHORE_H

#include <Arduino.h>

#define digitalToggle(outputPin)	\
    *portInputRegister(digitalPinToPort(outputPin)) |= digitalPinToBitMask(outputPin)

extern void openDrain (uint8_t pin, bool state) {
    volatile uint8_t *reg = portModeRegister(digitalPinToPort(pin));
    uint8_t bit = digitalPinToBitMask(pin);
    if (state) *reg &= ~bit;
    else       *reg |= bit;
}

extern float getThermistor (uint8_t pin, float Tb, float Ta, float Tr, float r1) {
    int Vt = analogRead(pin);
    float Rt = r1 * (1023.0 / (float)Vt - 1.0);
    float Tp = (1 / ( (log(Rt / Tr) ) / Tb + 1 / (273.15 + Ta) )) - 273.15;
    return Tp;
}

#endif
#endif

// end of header