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

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t dtoh (const uint8_t);
extern uint8_t htod (const uint8_t);

#ifdef __cplusplus
}
#endif

#endif

// end of header