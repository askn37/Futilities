/***************
 *
 * IntervalEvent - interval event class sample
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>
#include <chore.h>
#include "IntervalEvent.h"

#define LED_BUILTIN 13

IntervalEvent event;

int blinkId;

void fine (void) {
	Serial.println("Fine");
	event.clear(blinkId);
}
void third (void) {
	Serial.println("Third");
	event.setTimeout(fine, 2000);
}

void second (void) {
	Serial.println("Second");
	event.setTimeout(third, 2000);
}

void fist (void) {
	Serial.println("First");
	event.setTimeout(second, 2000);
}

void blink (void) {
	digitalToggle(LED_BUILTIN);
}

void setup (void) {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	Serial.println(F("Startup"));
	event.setTimeout(fist, 2000);
	blinkId = event.setInterval(blink, 200);
	event.setTimelimit(10000);
}

void loop (void) {
	event.yield();
	if (event.timeup()) Serial.println(F("Timeup"));
}

// end of code