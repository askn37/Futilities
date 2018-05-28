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

int IntervalEvent::setInterval (void (*userFunc)(void), uint32_t interval) {
	items++;
	void *newList = realloc(eventList, sizeof(interval_t) * items);
	if (newList == NULL) {
		items--;
		return 0;
	}
	eventList = (interval_t*) newList;
	int idx = items - 1;
	eventList[idx].interval = interval;
	eventList[idx].ms = millis();
	eventList[idx].timeout = 0;
	eventList[idx].id = ++serial;
	eventList[idx].event = userFunc;
	if (eventList[idx].id == 0) eventList[idx].id = ++serial;
	return eventList[idx].id;
}

int IntervalEvent::setTimeout (void (*userFunc)(void), uint32_t interval) {
	int eventId = this->setInterval(userFunc, interval);
	if (eventId) eventList[items - 1].timeout = 1;
	return eventId;
}

bool IntervalEvent::clear (int eventId) {
	int m = 0;
	if (items > 0) {
		for (int i = 0; i < items; i++) {
			if (eventList[i].id == eventId) continue;
			if (m != i) eventList[m] = eventList[i];
			m++;
		}
		if (items != m) {
			size_t s = sizeof(interval_t) * --items;
			if (s == 0) s = 4;
			void *newList = realloc(eventList, s);
			if (newList != NULL) eventList = (interval_t*) newList;
			return true;
		}
	}
	return false;
}

bool IntervalEvent::yield (int eventId) {
	for (int i = 0; i < items; i++) {
		if ( (millis() - eventList[i].ms) >= eventList[i].interval ) {
			void (*userFunc)(void) = eventList[i].event;
			int f = eventList[i].id == eventId;
			if (!eventList[i].timeout) eventList[i].ms = millis();
			else this->clear(eventList[i--].id);
			userFunc();
			if (f) return true;
		}
	}
	return false;
}

// end of code
