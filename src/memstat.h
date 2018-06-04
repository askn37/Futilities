/***************
 *
 * memstat - memory status utilities
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#ifndef __MEMSTAT_H
#define __MEMSTAT_H

#include <Arduino.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

extern size_t memFreeSize (void);

#ifdef __cplusplus
}
#endif

#endif

// end of header