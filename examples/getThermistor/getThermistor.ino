/***************
 *
 * getThermistor - Futilities toolbox sample
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>

// #include <Futilities.h>
#include <chore.h>

#define CONSOLE_BAUD 9600

void setup (void) {
    while (!Serial);
    Serial.begin(CONSOLE_BAUD);
    Serial.println(F("Startup"));
}

void loop (void) {
	float To = getThermistor(A0, 3950.0, 25.0, 20.0, 20.0);

	Serial.print(To, 2);
	Serial.println(F(" C"));

    delay(5000);
}

// end of code
