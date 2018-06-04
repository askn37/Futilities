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

#ifndef __ADCOMP_H
#define __ADCOMP_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void adcSetup (uint8_t data);
uint16_t adc (void);

extern uint16_t getVcc (void);

#ifdef __cplusplus
}
#endif

#endif

// end of header