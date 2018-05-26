/***************
 *
 * halt - halt and software reboot "Watchdog timer" utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>
#include "halt.h"

volatile uint8_t __wdt_count = 0;
static volatile void __wdt_vect_empty (void) { __wdt_count++; }
volatile void (*__wdt_vect)(void);
ISR(WDT_vect) { __wdt_vect(); }
// uint8_t getWdtCount (void) { return __wdt_count; }

uint16_t halt (uint16_t _sec, uint8_t _mode) {
    uint16_t _count = _sec;
    noInterrupts();
    __wdt_vect = __wdt_vect_empty;
    set_sleep_mode(_mode);
    if (_count == 0) {
        #ifdef WDTO_8S
        wdtStart(WDTO_8S);
        #else
        wdtStart(WDTO_2S);
        #endif
        while (_halt());
    }
    else {
        wdtStart(WDTO_1S);
        while (_count > 0 && _halt()) _count--;
    }
    sleep_disable();
    wdtDisable();
    wdtReset();
    interrupts();
    return _count;
}

bool _halt (void) {
    uint8_t _wdt = __wdt_count;
    sleep_enable();
    #if defined(BODS) && defined(BODSE)
    // Leonardo not used
    sleep_bod_disable();
    #endif
    interrupts();
    sleep_cpu();
    noInterrupts();
    return (_wdt != __wdt_count);
}

void reboot (void) {
    wdtEnable(WDTO_15MS);
    interrupts();
    while (true);
}

void wdtAttach (volatile void(*callback)(void)) {
    __wdt_vect = callback;
}

// end of code
