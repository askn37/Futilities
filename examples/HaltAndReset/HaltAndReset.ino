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
#include <avr/power.h>

// #include <Futilities.h>
#include <halt.h>

#define CONSOLE_BAUD 9600

//
// PCINTn_vect setup sample
//
#define PCINTPIN 6       // D6 --[ PUSH BUTTON ]-- GND

#define PCMSK *digitalPinToPCMSK(PCINTPIN)
#define PCINT digitalPinToPCMSKbit(PCINTPIN)
#define PCIE  digitalPinToPCICRbit(PCINTPIN)
#define PCPIN *portInputRegister(digitalPinToPort(PCINTPIN))

#if (PCIE == 0)
#define PCINT_vect PCINT0_vect
#elif (PCIE == 1)
#define PCINT_vect PCINT1_vect
#elif (PCIE == 2)
#define PCINT_vect PCINT2_vect
#else
#error This board doesnt support PCINTPIN ?
#endif

EMPTY_INTERRUPT(PCINT_vect)     // = ISR(PCINT_vect) {}

volatile uint16_t wdt_count = 0;
volatile void ISR_wdt (void) { wdt_count++; }

void setup (void) {
    pinMode(PCINTPIN, INPUT_PULLUP);

    while (!Serial);
    Serial.begin(CONSOLE_BAUD);
    Serial.println(F("Startup"));

    PCMSK |= _BV(PCINT);
    PCIFR |= _BV(PCIE);
    PCICR |= _BV(PCIE);
}

void loop (void) {

    //// watchdog timeout reset sample
    // uint8_t i = 0;
    // interrupts();
    // wdtEnable(WDTO_8S);
    // while (i += 16) {
    //     Serial.println(i * 30);
    //     delay(i * 30);
    //     wdtReset();
    // }
    // wdtDisable();

    Serial.print(F("millis: "));
    Serial.println(millis());

    // 10 sec halt 10 loops
    for (int i = 0; i < 10; i++) {
        Serial.print(F("sleep: "));
        Serial.println(i);
        Serial.flush();

        uint16_t rs = halt(10);     // remain seconds

        Serial.println(F("wakeup"));
        Serial.print(F("remain: "));
        Serial.println(rs);
        Serial.print(F("millis: "));
        Serial.println(millis());

        wdtAttach(ISR_wdt);
        wdtStart(WDTO_15MS);
        delay(1000);
        wdtReset();

        Serial.print(F("wdt_count: "));
        Serial.println(wdt_count);
        Serial.println();
    }

    // after mcu reset

    Serial.print(F("Reboot"));
    Serial.println();
    Serial.println();
    Serial.flush();

    reboot();
}

// end of code

