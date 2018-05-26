/***************
 *
 * Vcc - Futilities toolbox sample
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>

// #include <Futilities.h>
#include <adcomp.h>

#define CONSOLE_BAUD 9600

void setup (void) {
    while (!Serial);
    Serial.begin(CONSOLE_BAUD);
    Serial.println(F("Startup"));
}

void loop (void) {
    uint16_t vcc = getVcc();

    Serial.print(F("Vcc: "));
    Serial.print(vcc / 1000.0);
    Serial.print(F(" volt"));
    Serial.println();

    delay(5000);
}

// end of code
