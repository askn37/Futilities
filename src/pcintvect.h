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

#ifndef __PCINTVECT_H
#define __PCINTVECT_H

#include <Arduino.h>

extern "C" void attachPCInterrupt (uint8_t, void (*)(void) = NULL);
extern "C" void detachPCInterrupt (uint8_t);

#endif

// end of header