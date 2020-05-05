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

#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
#include "avr/bitsconv.h"
// #elif defined(ARDUINO_ARCH_SAM)
// #include "sam/bitsconv.h"
// #elif defined(ARDUINO_ARCH_SAMD)
// #include "samd/bitsconv.h"
// #elif defined(ARDUINO_ARCH_STM32F4)
// #include "stm32f4/bitsconv.h"
// #elif defined(ARDUINO_ARCH_NRF52)
// #include "nrf52/bitsconv.h"
// #elif defined(ARDUINO_ARCH_MBED)
// #include "mbed/bitsconv.h"
#else
#warning "This library only supports boards with an AVR or MEGAAVR processor."
#endif
