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

#if defined(ARDUINO_ARCH_AVR)
#include "avr/adcomp.h"
// #elif defined(ARDUINO_ARCH_SAM)
// #include "sam/adcomp.h"
// #elif defined(ARDUINO_ARCH_SAMD)
// #include "samd/adcomp.h"
// #elif defined(ARDUINO_ARCH_STM32F4)
// #include "stm32f4/adcomp.h"
// #elif defined(ARDUINO_ARCH_NRF52)
// #include "nrf52/adcomp.h"
// #elif defined(ARDUINO_ARCH_MEGAAVR)
// #include "megaavr/adcomp.h"
// #elif defined(ARDUINO_ARCH_MBED)
// #include "mbed/adcomp.h"
#else
#warning "This library only supports boards with an AVR processor."
#endif
