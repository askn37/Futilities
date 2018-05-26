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

#include <Arduino.h>
#include "memstat.h"

size_t memFreeSize (void) {
    uint8_t *_heapptr, *_stackptr;
    _heapptr = (uint8_t*)malloc(4);         // use stackptr temporarily
    if (_heapptr == NULL) return (0);
    free(_heapptr);                         // free up the memory again
    _stackptr = (uint8_t*)(SP);             // save value of stack pointer
    return (_stackptr - _heapptr);
}

// end of code
