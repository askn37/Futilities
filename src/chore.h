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

#ifndef __CHORE_H
#define __CHORE_H

#include <Arduino.h>

#define digitalToggle(outputPin)	\
    *portInputRegister(digitalPinToPort(outputPin)) |= digitalPinToBitMask(outputPin)

#endif

// end of header