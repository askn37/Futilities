/***************
 *
 * pcintvect - PCINTn_vect helper utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>
#include "pcintvect.h"

volatile void __empty (void) {}
volatile void (*__pcint_vect[4])(void) = {__empty, __empty, __empty, __empty};

#ifndef SoftwareSerial_h
#if defined(PCINT0_vect)
ISR(PCINT0_vect) { __pcint_vect[0](); }
#endif
#if defined(PCINT1_vect)
ISR(PCINT1_vect) { __pcint_vect[1](); }
#endif
#if defined(PCINT2_vect)
ISR(PCINT2_vect) { __pcint_vect[2](); }
#endif
#if defined(PCINT3_vect)
ISR(PCINT3_vect) { __pcint_vect[3](); }
#endif
#endif

void attachPCInterrupt (uint8_t interruptPin, void (*userFunc)(void)) {
    int _pcint = digitalPinToPCICRbit(interruptPin);
    __pcint_vect[_pcint & 3] = userFunc != NULL ? (volatile void (*)(void)) userFunc : (volatile void (*)(void)) __empty;
    *digitalPinToPCMSK(interruptPin) |= _BV(digitalPinToPCMSKbit(interruptPin));
    PCIFR |= _BV(digitalPinToPCICRbit(interruptPin));
    PCICR |= _BV(digitalPinToPCICRbit(interruptPin));
}

void detachPCInterrupt (uint8_t interruptPin) {
    int _pcint = digitalPinToPCICRbit(interruptPin);
    __pcint_vect[_pcint & 3] = (volatile void (*)(void)) __empty;
    *digitalPinToPCMSK(interruptPin) &= ~_BV(digitalPinToPCMSKbit(interruptPin));
    PCIFR &= ~_BV(digitalPinToPCICRbit(interruptPin));
    PCICR &= ~_BV(digitalPinToPCICRbit(interruptPin));
}

// end of code
