/***************
 *
 * FreeMemory - Futilities toolbox sample
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>

// #include <Futilities.h>
#include <memstat.h>

#define CONSOLE_BAUD 9600

void setup (void) {
    Serial.begin(CONSOLE_BAUD);

    Serial.print(F("memFreeSize: "));

    size_t freeSize = memFreeSize();

    Serial.print(freeSize, DEC);

    Serial.println(F(" bytes"));
}

void loop (void) {
}

// end of code
