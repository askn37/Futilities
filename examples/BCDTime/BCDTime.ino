/***************
 *
 * BCDTime - Futilities toolbox sample
 *
 * target architectures: Atmel AVR (ATmega 328P, 1284P and other)
 *
 * release site: https://github.com/askn37/Futilities
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>
#include <time.h>
#include <bcdtime.h>

void setup (void) {
    Serial.begin(9600);

    bcddate_t t_date = 0x20180530U;
    bcdtime_t t_time = 0x234355U;
    time_t t_epoch = bcdToEpoch(t_date, t_time);

    Serial.print(F("bcdToEpoch: "));

    Serial.print(t_date, HEX);
    Serial.write(' ');
    Serial.print(t_time, HEX);
    Serial.write(' ');
    Serial.print(t_epoch, DEC);
    Serial.println();

    t_date = epochToBcdDate(t_epoch);

    Serial.print(F("epochToBcdDate: "));

    Serial.print(t_date, HEX);
    Serial.write(' ');
    Serial.print(epochToWeekday(t_epoch), DEC);
    Serial.write(' ');
    Serial.print(bcdDateToWeekday(t_date), DEC);
    Serial.println();

    Serial.println();

    bcddatetime_t t_bcddatetme = epochToBcd(t_epoch);

    Serial.print(F("epochToBcd: "));
    Serial.print(t_bcddatetme.date, HEX);
    Serial.write(' ');
    Serial.print(bcdTimeToTimeString(t_bcddatetme.time));
    Serial.println();

    date_t t_mjd = bcdDateToMjd(t_date);

    Serial.print(F("bcdDateToMjd: "));

    Serial.print(t_mjd, DEC);
    Serial.write(' ');
    Serial.print(mjdToWeekday(t_mjd), DEC);
    Serial.println();

    Serial.println();

    Serial.println(F("epochToBcdDate: "));

    Serial.println(epochToBcdDate(0), HEX);
    Serial.println(epochToBcdDate(1234567890U), HEX);
    Serial.println(epochToBcdDate(0x80000000U), HEX);
    Serial.println(epochToBcdDate(2345678901U), HEX);
    Serial.println(epochToBcdDate(3456789012U), HEX);
    Serial.println(epochToBcdDate(0xFFFFFFFFU), HEX);

    Serial.println();

    Serial.println(F("bcdDateToMjd: "));

    Serial.println(bcdDateToMjd(t_date), DEC);
    Serial.println(bcdDateToMjd(0x00010101), DEC);
    Serial.println(bcdDateToMjd(0x18581117), DEC);
    Serial.println(bcdDateToMjd(0x19700101), DEC);
    Serial.println(bcdDateToMjd(0x20380119), DEC);
    Serial.println(bcdDateToMjd(0x21060207), DEC);
    Serial.println(bcdDateToMjd(0x99991231), DEC);
    Serial.println(bcdDateToMjd(0xF9991231), DEC);
    Serial.println();

    Serial.println(F("mjdToBcdDate: "));

    Serial.println(mjdToBcdDate(t_mjd), HEX);
    Serial.println(mjdToBcdDate(0), HEX);
    Serial.println(mjdToBcdDate(40587), HEX);
    Serial.println(mjdToBcdDate(2973483), HEX);
    Serial.println(mjdToBcdDate(5164938), HEX);
    Serial.println();

    Serial.println(F("mjdToEpoch: "));

    Serial.println(mjdToEpoch(t_mjd), DEC);
    Serial.println(mjdToEpoch(0), DEC);
    Serial.println(mjdToEpoch(40587), DEC);
    Serial.println(mjdToEpoch(65442), DEC);
    Serial.println(mjdToEpoch(90297), DEC);
    Serial.println();

}

void loop (void) {

}

// end of code
