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
#include "avr/chore.h"
// #elif defined(ARDUINO_ARCH_SAM)
// #include "sam/chore.h"
// #elif defined(ARDUINO_ARCH_SAMD)
// #include "samd/chore.h"
// #elif defined(ARDUINO_ARCH_STM32F4)
// #include "stm32f4/chore.h"
// #elif defined(ARDUINO_ARCH_NRF52)
// #include "nrf52/chore.h"
// #elif defined(ARDUINO_ARCH_MBED)
// #include "mbed/chore.h"
#else
#warning "This library only supports boards with an AVR or MEGAAVR processor."
#endif
