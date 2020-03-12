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
#if defined(REFS0)

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void adcSetup (uint8_t data);
uint16_t adc (void);

extern uint16_t getVcc (void);

extern void openDrain (uint8_t, bool);

extern float getThermistor (uint8_t, float, float, float, float);

#ifdef __ADCOMP_ENABLE_ACP
void acpSetup (void);
extern void acpAttachInterrupt (uint8_t, void(*)(void), int);
#endif

#ifdef __cplusplus
}
#endif

#endif
#else
#warning nosupported model; REFS0 bit in getVcc()
#endif

// end of header