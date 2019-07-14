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

eventid_t IntervalEvent::setInterval (
    eventid_t userFunc,
    uint32_t interval,
    uint32_t offset,
    bool oneshot ) {

    void *_newList = NULL;
    uint32_t launch = millis();
    int used = -1;
    if (_items == 0) {
        _newList = malloc(sizeof(interval_t));
        if (_newList == NULL) return NULL;
        used = 0;
        _items = 1;
        _eventList = (interval_t*) _newList;
    }
    else {
        for (int i = 0; i < _items; i++) {
            if (_eventList[i].eventid == userFunc) used = i;
        }
        if (used < 0) {
            used = _items;
            _newList = realloc(_eventList,
                sizeof(interval_t) * (_items + 1));
            if (_newList == NULL) return NULL;
            _items++;
            _eventList = (interval_t*) _newList;
        }
    }
    if (offset) {
        launch = launch - interval + offset;
    }
    _eventList[used].interval = interval;
    _eventList[used].ms = launch;
    _eventList[used].ones = oneshot;
    _eventList[used].eventid = userFunc;
    return userFunc;
}

bool IntervalEvent::clear (eventid_t eventId) {
    int m = 0;
    if (_items > 0) {
        if (eventId == NULL) {
            _items = 0;
            free(_eventList);
            _eventList = NULL;
            return true;
        }
        for (int i = 0; i < _items; i++) {
            if (_eventList[i].eventid == eventId) continue;
            if (m != i) _eventList[m] = _eventList[i];
            m++;
        }
        if (_items != m) {
            _items--;
            if (_items == 0) {
                free(_eventList);
                _eventList = NULL;
            }
            else {
                void *_newList = realloc(_eventList,
                    sizeof(interval_t) * _items);
                if (_newList != NULL) _eventList = (interval_t*) _newList;
            }
            return true;
        }
    }
    return false;
}

bool IntervalEvent::isEvent (eventid_t eventId) {
    for (int i = 0; i < _items; i++) {
        if (_eventList[i].eventid == eventId) return true;
    }
    return false;
}

bool IntervalEvent::yield (eventid_t eventId) {
    for (int i = 0; i < _items; i++) {
        if ( (millis() - _eventList[i].ms) >= _eventList[i].interval ) {
            eventid_t userFunc = _eventList[i].eventid;
            int f = _eventList[i].eventid == eventId;
            if (_eventList[i].ones) this->clear(_eventList[i--].eventid);
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
