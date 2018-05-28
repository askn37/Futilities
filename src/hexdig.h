/***************
 *
 * hexdig - hex digit convert helper utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#ifndef __HEXDIG_H
#define __HEXDIG_H

#include <Arduino.h>

extern "C" uint8_t dtoh (const uint8_t);
extern "C" uint8_t htod (const uint8_t);

#endif

// end of header