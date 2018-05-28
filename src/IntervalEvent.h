/***************
 *
 * InteralEvent - interval event class
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#ifndef __INTERVALEVENT_H
#define __INTERVALEVENT_H

#include <Arduino.h>

class IntervalEvent {
private:

    typedef struct {
        uint32_t interval;
        uint32_t ms;
        int id;
        bool timeout;
        void (*event)(void);
    } interval_t;

    interval_t *eventList;
    int items = 0;
    int serial = 1;

public:
    IntervalEvent (void) { items = 0; eventList = (interval_t*) malloc(4); }
    ~IntervalEvent (void) { free(eventList); }

    int setInterval (void (*userFunc)(void), uint32_t interval);
    int setTimeout (void (*userFunc)(void), uint32_t interval);
    bool clear (int eventId);

    bool yield (int eventId = 0);
};

#endif

// end of header