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
        bool ones;
        void (*event)(void);
    } interval_t;

    interval_t *_eventList;
    uint32_t _timeup;
    uint32_t _ms = 0;
    int _items = 0;
    int _serial = 1;

public:
    IntervalEvent (void) { _items = 0; _eventList = (interval_t*) malloc(4); }
    ~IntervalEvent (void) { free(_eventList); }

    int setInterval (void (*)(void), uint32_t, bool = false);
    inline int setTimeout (void (*userFunc)(void), uint32_t interval) {
        return setInterval(userFunc, interval, true); 
    }
    bool clear (int);
    bool yield (int = 0);

    bool intime (uint32_t = 0);
    inline bool timeup (void) { return !intime(); }
    inline bool setTimelimit (uint32_t interval) { return intime(interval); }
};

#endif

// end of header