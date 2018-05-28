/***************
 *
 * bitsconv - bits convert helper utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#ifndef __BITSCONV_H
#define __BITSCONV_H

#include <Arduino.h>
#include <avr/pgmspace.h>

extern "C" uint16_t wbits (const uint8_t);
extern "C" uint8_t rbits (const uint8_t);

#endif

// end of header