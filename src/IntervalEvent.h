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

typedef void (*eventid_t)(void);

class IntervalEvent {
private:

    struct interval_t {
        uint32_t interval;
        uint32_t ms;
        eventid_t eventid;
        bool ones;
    };

    interval_t *_eventList;
    uint32_t _timeup;
    uint32_t _ms = 0;
    int _items = 0;
    int _serial = 1;

public:
    IntervalEvent (void) { _items = 0; _eventList = NULL; }
    ~IntervalEvent (void) { if (_eventList) free(_eventList); }

    eventid_t setInterval (eventid_t, uint32_t, uint32_t = 0, bool = false);
    inline eventid_t setTimeout (eventid_t userFunc, uint32_t interval) {
        return setInterval(userFunc, interval, 0, true);
    }
    bool isEvent (eventid_t);
    bool clear (eventid_t = NULL);
    bool yield (eventid_t = NULL);

    bool intime (uint32_t = 0);
    inline bool timeup (void) { return !intime(); }
    inline bool setTimelimit (uint32_t interval) { return intime(interval); }
};

#endif

// end of header