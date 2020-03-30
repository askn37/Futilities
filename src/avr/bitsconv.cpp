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
#include <Arduino.h>
#include "avr/bitsconv.h"

const uint8_t PROGMEM _wide_bits_PGM[] = {
    0b00000000,
    0b00000011,
    0b00001100,
    0b00001111,
    0b00110000,
    0b00110011,
    0b00111100,
    0b00111111,
    0b11000000,
    0b11000011,
    0b11001100,
    0b11001111,
    0b11110000,
    0b11110011,
    0b11111100,
    0b11111111
};
uint16_t wbits (const uint8_t bits) {
    uint16_t _wide = (pgm_read_byte(_wide_bits_PGM + (bits >> 4)) << 8)
                   |  pgm_read_byte(_wide_bits_PGM + (bits & 15));
    return _wide;
}

const uint8_t PROGMEM _reverse_bits_PGM[] = {
    0b0000,
    0b1000,
    0b0100,
    0b1100,
    0b0010,
    0b1010,
    0b0110,
    0b1110,
    0b0001,
    0b1001,
    0b0101,
    0b1101,
    0b0011,
    0b1011,
    0b0111,
    0b1111
};
uint8_t rbits (const uint8_t bits) {
    uint8_t _rev = pgm_read_byte(_reverse_bits_PGM + (bits >> 4))
                | (pgm_read_byte(_reverse_bits_PGM + (bits & 15)) << 4);
    return _rev;
}
#endif

// end of code
