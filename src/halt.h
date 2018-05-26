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

#ifndef __HALT_H
#define __HALT_H

#include <Arduino.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#define wdt_start(value)   \
__asm__ __volatile__ (  \
    "in __tmp_reg__,__SREG__"   "\n\t" \
    "cli"                       "\n\t" \
    "wdr"                       "\n\t" \
    "sts %0,%1"                 "\n\t" \
    "out __SREG__,__tmp_reg__"  "\n\t" \
    "sts %0,%2"                 "\n\t" \
    : /* no outputs */  \
    : "M" (_SFR_MEM_ADDR(_WD_CONTROL_REG)), \
      "r" (_BV(_WD_CHANGE_BIT) | _BV(WDE)), \
      "r" ((uint8_t) ((value & 0x08 ? _WD_PS3_MASK : 0x00) | \
          _BV(WDIE) | (value & 0x07)) ) \
    : "r0"  \
)

#define wdtStart(value)     wdt_start(value)
#define wdtEnable(value)    wdt_enable(value)
#define wdtDisable()        wdt_disable()
#define wdtStop()           wdt_disable()
#define wdt_stop()          wdt_disable()
#define wdtReset()          wdt_reset()

bool _halt (void);
static volatile void __wdt_vect_empty (void);
extern "C" volatile void (*__wdt_vect)(void);
extern "C" void reboot (void);
extern "C" void wdtAttach (volatile void(*)(void) = __wdt_vect_empty);
// extern "C" uint8_t getWdtCount (void);

#if defined(BODS) && defined(BODSE)
extern "C" uint16_t halt (uint16_t = 0, uint8_t = SLEEP_MODE_PWR_DOWN);
#else
extern "C" uint16_t halt (uint16_t = 0, uint8_t = SLEEP_MODE_IDLE);
#endif

#endif

// end of header
