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

#include <Arduino.h>
#include "gpio.h"

void digitalToggle (uint8_t outputPin) {
    *portInputRegister(digitalPinToPort(outputPin)) |= digitalPinToBitMask(outputPin);
}

// end of code
