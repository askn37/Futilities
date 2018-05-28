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

#include <Arduino.h>
#include "hexdig.h"

uint8_t dtoh (const uint8_t dint) {
    uint8_t hchar = dint & 15;
    if (hchar >= 10) hchar += 'A' - '0' - 10;
    return hchar + '0';
}

uint8_t htod (const uint8_t hchar) {
    uint8_t dint = hchar - '0';
    if (dint >= 17) dint -= 7;
    if (dint >= 42) dint -= 32;
    return dint & 15;
}

// end of code
