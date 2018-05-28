/***************
 *
 * HaltAndReset - Futilities toolbox sample
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>
// #include <avr/power.h>

// #include <Futilities.h>
#include <halt.h>
#include <pcintvect.h>
#include <gpio.h>

#define CONSOLE_BAUD 9600

#define LED_BUILTIN 13
#define PCINTPIN 8       // D8 --[ PUSH BUTTON ]-- GND

volatile uint16_t wdt_count = 0;
void ISR_wdt (void) {
    wdt_count++;

    digitalToggle(LED_BUILTIN);
}

void setup (void) {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PCINTPIN, INPUT_PULLUP);

    Serial.begin(CONSOLE_BAUD);
    Serial.println(F("Startup"));

    attachPCInterrupt(PCINTPIN, NULL);
}

void loop (void) {

    Serial.print(F("millis: "));
    Serial.println(millis());

    // 10 sec halt 5 loops
    for (int i = 0; i < 5; i++) {
        Serial.println(F("led blink"));
        wdtAttachInterrupt(ISR_wdt);
        wdtStart(WDTO_60MS);
        delay(2000);
        wdtReset();
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print(F("wdt_count: "));
        Serial.println(wdt_count);
        Serial.println();

        Serial.print(F("sleep: "));
        Serial.println(i);
        Serial.flush();

        uint16_t rs = halt(10);     // remain seconds

        Serial.println(F("wakeup"));
        Serial.print(F("remain: "));
        Serial.println(rs);
        Serial.print(F("millis: "));
        Serial.println(millis());
    }

    // after mcu reset
    Serial.print(F("Reboot"));
    Serial.println();
    Serial.println();
    Serial.flush();

    reboot();
}

// end of code

