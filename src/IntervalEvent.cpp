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

#include <Arduino.h>
#include "IntervalEvent.h"

int IntervalEvent::setInterval (void (*userFunc)(void), uint32_t interval, bool oneshot) {
    int i;
    // Search for unique new id
    _serial += 2;
    for (i = 0; i < _items; i++) {
        if (_eventList[i].id == _serial) {
            _serial += 2;
            i = -1;
        }
    }
    void *_newList = realloc(_eventList, 4 | sizeof(interval_t) * (_items + 1));
    if (_newList == NULL) return 0;
    _eventList = (interval_t*) _newList;
    i = _items;
    _items++;
    _eventList[i].interval = interval;
    _eventList[i].ms = millis();
    _eventList[i].ones = oneshot;
    _eventList[i].id = _serial;
    _eventList[i].event = userFunc;
    return _eventList[i].id;
}

bool IntervalEvent::clear (int eventId) {
    int m = 0;
    if (_items > 0) {
        for (int i = 0; i < _items; i++) {
            if (_eventList[i].id == eventId) continue;
            if (m != i) _eventList[m] = _eventList[i];
            m++;
        }
        if (_items != m) {
            void *_newList = realloc(_eventList, 4 | sizeof(interval_t) * --_items);
            if (_newList != NULL) _eventList = (interval_t*) _newList;
            return true;
        }
    }
    return false;
}

bool IntervalEvent::yield (int eventId) {
    for (int i = 0; i < _items; i++) {
        if ( (millis() - _eventList[i].ms) >= _eventList[i].interval ) {
            void (*userFunc)(void) = _eventList[i].event;
            int f = _eventList[i].id == eventId;
            if (_eventList[i].ones) this->clear(_eventList[i--].id);
            else _eventList[i].ms = millis();
            userFunc();
            if (f) return true;
        }
    }
    return false;
}

bool IntervalEvent::intime (uint32_t interval) {
    if (interval) {
        _ms = interval;
        _timeup = millis();
    }
    else if (_ms > 0 && (millis() - _timeup) >= _ms) {
        _ms = 0;
        return false;
    }
    return true;
}

// end of code
