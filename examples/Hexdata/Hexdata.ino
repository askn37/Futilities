/***************
 *
 * Hexdata - Hexdata conversion sample
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>

// #include <Futilities.h>
#include <hexdig.h>
#include <bitsconv.h>

#define CONSOLE_BAUD 9600

void setup (void) {
    while (!Serial);
    Serial.begin(CONSOLE_BAUD);

	char hexdata[] = "48494a4b";
	for (int i = 0; i < sizeof(hexdata); i++) {
		Serial.print( htod(hexdata[i]), DEC);
		Serial.write(' ');
	}
	Serial.println();

	for (int i = 0; i < 256; i++) {
		Serial.write(dtoh(i >> 4));
		Serial.write(dtoh(i & 15));
		Serial.write(' ');
		if (i % 16 == 15) Serial.println();
	}
	Serial.println();

	for (int i = 0; i < 256; i++) {
		Serial.print(wbits(i), HEX);
		Serial.write(' ');
		if (i % 16 == 15) Serial.println();
	}
	Serial.println();

	for (int i = 0; i < 256; i++) {
		Serial.print(rbits(i), HEX);
		Serial.write(' ');
		if (i % 16 == 15) Serial.println();
	}
	Serial.println();

}

void loop (void) {}

// end of code
