/***************
 *
 * halt - halt and software reboot "Watchdog timer" utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#if defined(ARDUINO_ARCH_AVR)
#include "avr/halt.h"
// #elif defined(ARDUINO_ARCH_SAM)
// #include "sam/halt.h"
// #elif defined(ARDUINO_ARCH_SAMD)
// #include "samd/halt.h"
// #elif defined(ARDUINO_ARCH_STM32F4)
// #include "stm32f4/halt.h"
// #elif defined(ARDUINO_ARCH_NRF52)
// #include "nrf52/halt.h"
// #elif defined(ARDUINO_ARCH_MEGAAVR)
// #include "megaavr/halt.h"
// #elif defined(ARDUINO_ARCH_MBED)
// #include "mbed/halt.h"
#else
#warning "This library only supports boards with an AVR processor."
#endif
