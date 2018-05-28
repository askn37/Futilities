/***************
 *
 * gpio - GPIO helper utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#ifndef __GPIO_H
#define __GPIO_H

#include <Arduino.h>

extern "C" void digitalToggle (uint8_t);

#endif

// end of header