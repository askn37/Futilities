# Futilities

あまり有益でもない Arduino 向けユーティリティツールボックス

多くはコピー＆ペーストで足りるようなコードの断片を
インクルード仕立てにしただけのもので、
半ば備忘録のようなライブラリである。

## Arduino IDE への導入

1. .ZIPアーカイブをダウンロードする。[Click here](https://github.com/askn37/Futilities/archive/master.zip)

2. ライブラリマネージャで読み込む

    スケッチ -> ライブラリをインクルード -> .ZIP形式のライブラリをインストール...

## 使い方

```c
// まとめて全部
#include <Arduino.h>
#include <Futilities.h>

// あるいは必要なヘッダファイルだけ
#include <adcomp.h>           // AD比較変換
#include <bcdtime.h>          // BCD日時変換
#include <bitsconv.h>         // ビット変換
#include <chore.h>            // 雑役マクロ
#include <halt.h>             // 省電力休止と Watchdog Timer
#include <hexdig.h>           // 16進変換
#include <IntervalEvent.h>    // インターバルイベントクラス
#include <memstat.h>          // メモリ情報
#include <pcintvect.h>        // ピン変化割込
```

# リファレンス

ほとんどは C言語関数形式である。

- [AD比較変換](#adcomph)
- [BCD日時変換](#bcdtimeh)
- [ビット変換](#bitsconvh)
- [雑役マクロ](#choreh)
- [省電力休止と Watchdog Timer](#halth)
- [16進変換](#hexdigh)
- [インターバルイベントクラス](#IntervalEventh) (C++)
- [メモリ情報](#memstath)
- [ピン変化割込](#pcintvecth)

----

## adcomp.h

AD比較変換

依存性：

### uint16\_t getVcc (void)

VCC入力電圧を測定して返す。
返値は無符号整数で、精度は 1/1000 Volt の固定小数点3桁である。

```c
// #include <adcomp.h>

uint16_t vcc = getVcc();
Serial.print(vcc / 1000.0);
```

MCU（AVR）の VCC絶対定格は 6.0V なので、最大値は 6000 であろう。

----

## bcdtime.h

BCD日時変換

依存性：

主に RTCで扱われる BCDフォーマットのカレンダー型データと、
unix time（epoch）、および修正ユリウス通日（MJD）の相互変換を提供する。
これらは 64bit型変数が扱えない MCUで、
epochの範囲を超えた日数計算を助ける。

### typedef time\_t

unix time epochを表す時間データ型。
事前に time.h が読み込まれていなければ uint32_t で宣言される。

通常の（8bit系 AVR の）Arduino.h では、 time_t 型を定義していない。
これは time.h を読み込んでいないことによる。
そして time.h の定義では、
32bitあるいはそれ以上の環境では int64_t になっていることもあろう。
従って time_t == uint32_t では不整合が生じる場合は、
time.h を事前にインクルードしなければならない。

### typedef date\_t

修正ユリウス通日（Modified Julian Date : MJD）を扱う整数日数型。
元期 0 == 西暦1858年11月17日の正子 である。

ユリウス通日（元期 西暦 -4713年11月24日の正午）に変換するには
2400000.5 を引く。
ただし 単精度浮動小数変数（float型）では
精度不足によりこれを正しく扱える保証がなく
従って 8bit系 AVR では有用とはならない。
そもそも MJDがとある天文台で 1957年に考案された理由は、
ユリウス通日の桁数過剰による演算処理能力不足への対処である。

### typedef bcddate\_t

BCDカレンダー整数型の "年月日"。
これを 16進数型のパックドBCD表現とみなして
uint32_t へそのまま格納する。

```c
bcdtime_t bcddate = 0x20180530;   // 2018/05/30
```

表現可能範囲は 0x00000101～0x99991231 である。

ただし便宜上、最上位桁は 10～15 を A～F で表現することを許容するので、
0xF9991231 == 15999/12/31 とみなせば扱える。
一方でこのデータ型は符号を持たないので "紀元前" は扱えない。

現代的な MCUでは BCDの直値演算は非効率的なため、
年月日の四則演算を行うには一旦 10進数表現の  MJDに変換して行うことになる。

### typedef bcdtime\_t

BCDカレンダー整数型の "時分秒"。
これを 16進数型のパックドBCD表現とみなして
uint32_t へそのまま格納する。

```c
bcdtime_t bcdtime = 0x235959;   // 23:59:59
```

表現可能範囲は 0x00000000～0x00235959 である。

ただし便宜上、未使用の左端 BCD 2桁を "日数" として表現することを許容するので、
0x30235959 == 31日目の23:59:59 とみなせる。
さらに最上位桁は 10～15 を A～F で表現することを許容するので、
0xF9235959 == 160日目の23:59:59 とみなすこともできる。
しかしこのデータ型は符号を持たないので、負の時間表現はできない。

現代的な MCUでは BCDの直値演算は非効率的なため、
時分秒の四則演算を行うには一旦 10進数表現の epochに変換して行うことになる。

### struct bcddatetime\_t

BCD年月日とBCD時分秒を併せ持つ（64bit幅の）構造体。

```c
struct bcddatetime_t {
    bcddate_t date;
    bcdtime_t time;
};
```

time.t で宣言される struct tm 構造体のような各種の付帯情報は持たない。
しかし time_t や tm よりも表現可能な年月日は広い。

この構造体がメモリ上に展開されるとき、
リトルエンディアンになるかビッグエンディアンになるかは
MCU（とそのコンパイラ）の実装に依存する。

### uint8\_t dtob (const uint8\_t DECBYTE)

10進数の 0～99 を 16進数の BCD 0x00～0x99 に変換して返すバイト型関数。

### uint8\_t btod (const uint8\_t BCDBYTE)

16進数の BCD 0x00～0x99 を 10進数の 0～99 に変換して返すバイト型関数。

### uint16\_t wdtob (const uint16\_t DECWORD)

10進数の 0～9999 を 16進数の BCD 0x0000～0x9999 に変換して返すワード型関数。

### uint16\_t wbtod (const uint16\_t BCDWORD)

16進数の BCD 0x0000～0x9999 を 10進数の 0～9999 に変換して返すワード型関数。

### time\_t bcdToEpoch (const bcddate\_t BCDDATE, const bcdtime\_t BCDTIME)

BCDカレンダー型年月日および時分秒の 2引数を epoch に変換して返す。
epoch の有効範囲外は正しい値とならない。

### time\_t bcdDateTimeToEpoch (const bcddatetime\_t BCDDATETIME)

bcddatetime_t 構造体を引数にとり、epoch に変換して返す。
epoch の有効範囲外は正しい値とならない。

### date\_t bcdDateToMjd (const bcddate\_t BCDDATE)

BCD年月日を引数にとり、修正ユリウス通日（MJD）に変換して返す。

### uint8\_t bcdDateToWeekday (const bcddate\_t BCDDATE)

BCD年月日を引数にとり、その曜日を表す整数を返す。
表現範囲は 日曜 == 0 ～ 土曜 == 6 である。

### time\_t bcdTimeToSeconds (const bcdtime\_t BCDTIME)

BCD時分秒を引数にとり、epoch にして返す。
つまり 0x00235959 == 86399 となり、正子からの経過秒数となる。

便宜上、未使用の左端 BCD 2桁を "日数" として解釈することを許容するので、
"ある時点からの経過日時分秒" と扱うこともできる。

またこれは RTCの "アラーム日時分＋曜日" を表す BCD表現を
epoch とみなして変換するのにも役立つ。

### String bcdTimeToTimeString (const bcdtime\_t BCDTIME)

BCD時分秒を引数にとり、6桁の文字列表現にして返す。
0時台等の桁が不足する場合の左端は "0" で埋められる。
これは Stream.print() に
ゼロパディング機能がないことの補完処置を提供するためにある。

### bcddate\_t mjdToBcdDate (const date\_t MJD)

修正ユリウス通日（MJD）を引数にとり、BCD年月日 を返す。
BCD年月日の有効範囲外は正しい値とならない。

### uint8\_t mjdToWeekday (const date\_t MJD)

修正ユリウス通日（MJD）を引数にとり、その曜日を表す整数を返す。
表現範囲は 日曜 == 0 ～ 土曜 == 6 である。

### time\_t mjdToEpoch (const date\_t mjd);

修正ユリウス通日（MJD）を引数にとり、epoch にして返す。
時分秒は 00:00:00（正子） とするので 86400 の倍数となる。
有効範囲外は正しい値とならない。
表現可能範囲は以下のようになる。

    MJD 40587 == BCD 19700101 == Epoch 0
    MJD 65442 == BCD 20380119 == Epoch 0x7FFFD280 (24855 day)
    MJD 90297 == BCD 21060207 == Epoch 0xFFFFA500 (49710 day)

### date\_t epochToMjd (const time\_t EPOCH)

epoch を引数にとり、その年月日を 修正ユリウス通日（MJD）で返す。
時分秒は捨てられる。
mjdToEpoch() の逆変換である。

### bcddatetime\_t epochToBcd (const time\_t EPOCH)

epoch を引数にとり、bcddatetime_t 構造体に変換して返す。

### bcddate\_t epochToBcdDate (const time\_t EPOCH)

epoch を引数にとり、その年月日を BCD年月日に変換して返す。
時分秒は失われる。

### bcdtime\_t epochToBcdTime (const time\_t EPOCH)

epoch を引数にとり、その時分秒を BCD時分秒に変換して返す。
年月日は失われる。

### bcdtime\_t epochToBcdDayTime (const time\_t EPOCH)

epoch を引数にとり、その "日" 時分秒を BCD "日" 時分秒に変換して返す。
年月は失われる。

この関数は epochToBcdTime() と違って "日" も含めて返される。
これは RTCの "アラーム日時分＋曜日" を表す BCD表現を
time_t から作り出すのに役立つ。

### uint8\_t epochToWeekday (const time\_t EPOCH)

epoch を引数にとり、その曜日を表す整数を返す。
表現範囲は 日曜 == 0 ～ 土曜 == 6 である。

### String epochToTimeString (const time\_t EPOCH)

epoch を引数にとり、その時分秒を 6桁の文字列表現にして返す。
0時台等の桁が不足する場合の左端は "0" で埋められる。
これは Stream.print() に
ゼロパディング機能がないことの補完処置を提供するためにある。

----

## bitsconv.h

ビット変換

依存性：
<avr/pgmspace.h>

### uint16\_t wbits (const uint8\_t bits)

8bitの引数に対応する 16bitの "倍角" ビットパターンを返す。

```c
// #include <bitsconv.h>

uint16_t bits = wbits(0x00100101);    // 0b0000110000110011
```

### uint8\_t rbits (const uint8\_t bits)

8bitの引数に対応する 8bitの "鏡対称" ビットパターンを返す。

```c
// #include <bitsconv.h>

uint8_t bits = rbits(0x00100101);     // 0b10100100
```

----

## chore.h

雑役マクロ

依存性：

### digitalToggle(outputPin)

マクロ：
指定のピンの出力をトグル変化する。
ピンが OUTPUT に指定されているなら HIGH と LOW を、
INPUT に指定されているならプルアップ抵抗の ON と OFF を切り替える。

----

## halt.h

省電力休止と Watchdog Timer

依存性：
<avr/wdt.h>
<avr/sleep.h>

注意：
Leonardo（ATmega32U4）等の USB-UART内臓型では期待したとおりには動作しない。

### uint16\_t halt (uint16\_t SECONDS = 0, uint8\_t MODE = SLEEP\_MODE\_PWR\_DOWN)

指定した時間、MCU（AVR）を指定の休止モードで停止する。
第1引数は 1～65535 の秒数、または省略時 0 == 無期限である。
第2引数は cpu_sleep() に渡す定数を指定し、省略時は SLEEP_MODE_PWR_DOWN である。
停止中はタイマー計時等も停止し、
外部割込要因以外は受け付けない。
外部割込で停止状態が解除されると、1以上の残時間を返す。
ただし無期限停止解除の場合は常に 0を返す。

```c
// #include <halt.h>

Serial.println("Zzz...");
Serial.flush();

halt();   // 外部割込がない限りホールトし続ける

Serial.println("Wakeup!");

// 65535秒を超えて
// 最長で丸1日（86400秒）止めたい場合はカスケードする
halt(43200) || halt(43200);
```

停止中は BOD は無効にされるが、それ以外の（avr/sleep.h等による）省電力機能には関与しない。
時間計時は Watchdog Timer を使用しているため正確ではなく、10%あるいはぞれ以上の誤差がある。
タイムアウト周期は秒数指定時は WDTO_1S であるが、無期限時は WDTO_8S または WDTO_2S が使われる。

Leonardo（ATmega32U4）では常に USB-UART が割込を発生させているので、
USB-UART を停止しないと halt() は最大1秒以内に終了してしまう。
さらに USB-UART を一度止めると、
復帰させても ホストPCとの Serial接続が復帰しない
（reboot() も同様。Serial接続の復帰にはホストPC側での対応が必要）
従って halt() に限らず省電力休止関係の処置は、
この種の MCUではあまり実用的には使えない。

### void reboot (void)

MCU（AVR）を直ちに MCUデバイスのリセットする。
このMCUデバイスのリセットで ~RESET ピンは LOW にはならないため、
周辺機器まではリセットされない。

```c
// #include <halt.h>

reboot();
```

### void wdtAttachInterrupt (void(*CALLBACK)(void) = NULL)

WDT_vect にユーザ定義割込ルーチンを割り当てる。

```c
// #include <halt.h>

volatile uint16_t wdt_count = 0;
void ISR_wdt (void) {
  wdt_count++;
}
wdtAttachInterrupt(ISR_wdt);
wdtStart(WDTO_1S);
```

halt() 実行中以外は WDT_vect タイマーは使用されていないため、ユーザレベルで使用しても構わない。
このエントリは（halt.hで定義された）真の WDT_vect から call される。
ただし halt() を実行するたびにこの指定は上書きされて失われるので、
wdtAttachInterrupt() は逐次実行する必要がある。

引数に NULL を指定する（あるいは省略する）と wdtDetachInterrupt() と同義となる。

### void wdtDetachInterrupt (void)

wdtAttachInterrupt() で割り当てたユーザ定義割込ルーチンを取り消す。
Watchdog Timer は停止しない。
単に halt() が使用するデフォルト割込に戻されるだけである。
さらに言えば wdtAttachInterrupt(NULL) と同義である。

### void wdtStart (uint8\_t WDTO)

指定のタイムアウト周期で Watchdog Timer を開始する。
タイムアウトしても MCUデバイスのリセットは発生しない。
引数には（wdt.hで定義された）以下の定数が指定できる。

    WDTO_15MS WDTO_30MS WDTO_60MS WDTO_120MS
    WDTO_250MS WDTO_500MS WDTO_1S WDTO_2S WDTO_4S WDTO_8S

タイムアウトを止めるには wdtStop() を使う。

古い AVRでは WDTO_4S と WDTO_8S はサポートされないだろう。

別名として wdt_start() も使用できる。

### 補記

halt.h では、wdt.h の定義についても利便上以下の別名が使えるようになる。

    wdt_enable()  -> wdtEnable()
    wdt_disable() -> wdtDisable() or wdtStop() or wdt_stop()
    wdt_reset()   -> wdtReset()

wdtEnable() は MCUデバイスのリセットを伴うタイムアウト設定で、
タイムアウト時間内に wdtReset() を実行しなければ MCUデバイスのリセットが発生する。
wdtDisable() はタイムアウト周期を止めて MCUデバイスのリセットが発生しないようにする。

ここでは reset という用語が、
MCUデバイスのリセットを指すのではなく、wdtカウントの停止でもなく、
wdtカウントのリセット（最初から再カウント）であることには留意すべきだろう。

```c
// watchdog タイムアウトをわかりやすく試す例

uint8_t i = 0;
interrupts();
wdtEnable(WDTO_1S);       // これを WDTO_8S にすると次のループは完遂するだろう

while (i += 16) {
  Serial.println(i * 30);
  delay(i * 30);          // この delay() 中にタイムアウトすると MCUがリセットされる
  wdtReset();             // タイムアウト再カウント指示
}

wdtDisable();             // watchdog timer 停止
```

----

## hexdig.h

16進文字変換

依存性：

### uint8\_t dtoh (const uint8\_t dint)

引数の下位 4bitに対応する ASCII文字を返す。
結果は大文字になる。

```c
// #include <hexdig.h>

uint8_t H = dteh(15);           // 'F'
uint8_t h = dteh(15) | 0x20;    // 'f'
```

### uint8\_t htod (const uint8\_t hchar)

16進数を表す ASCII文字の引数に対応する 4bitの正数を返す。
レターケースには対応するが、
不正な文字の確認はしていない（何らかの値になる）

```c
// #include <hexdig.h>

uint8_t a = htod('b');          // 11
```

----

## IntervalEvent.h

タイマー非依存インターバルイベントクラス

依存性：
<stdlib.h>

### IntervalEvent (void)

コンストラクタ。
このクラスオブジェクトには、ひとつのタイムアップ・イベントと、
任意長の待機イベントキューを持つ。

待機イベントキューは任意のユーザ関数を登録し、
指定した経過時間後に実行させる。
（タイムアップ・イベントの使用例は後述）

```c
#include <IntervalEvent.h>

IntervalEvent event;

void setup (void) {
  event.setTimeout(oneShotEvent, 2000);
  event.setInterval(repeatEvent, 200);
}

void loop (void) {
  event.yield();
}
```

待機イベントキューは malloc()、realloc()、free() で実装されている。
登録可能イベント数には空きメモリがある限り上限はない。

### eventid\_t setInterval (void (*userFunc)(void), uint32\_t interval)

intervalミリ秒間隔で定期実行するイベントを作成し、成功ならイベントID（常に真）を返す。

おなじ関数ポインタを渡すと既存のイベントは上書きされる。

- eventid\_t型が示す イベントIDの実体は、userFunc関数ポインタそのものである。

### eventid\_t setTimeout (void (*userFunc)(void), uint32\_t interval)

intervalミリ秒後に一度だけ実行するイベントを作成し、成功ならイベントID（常に真）を返す。

おなじ関数ポインタを渡すと既存のイベントは上書きされる。

- eventid\_t型が示す イベントIDの実体は、userFunc関数ポインタそのものである。

### bool isEvent (eventid\_t eventId)

イベントIDを引数にとり、待機キューに見つかるなら真を返す。

### bool clear (eventid\_t eventId = NULL)

イベントIDを指定し、そのイベントを取り消す。
イベントが存在しなければ偽を返す。
引数を省略あるいは NULL を指定すると、全てのイベントを取り消す。

### bool yield (eventid\_t eventId = NULL)

引数を省略あるいは NULL を渡すと、所定の時間以上を経過したイベントを実行する。
引数にイベントIDを指定した場合は、そのイベントが実行されたときに真を返す。

### bool setTimelimit (uint32\_t interval)

タイムアップ・イベントの実行時間を intervalミリ秒で設定する。
常に真を返す。
このタイムアップ・イベントはクラスオブジェクトひとつにつき、ひとつだけ存在する。

### bool timeup (void)

setTimelimit() で設定した時間を経過していれば真を返す。
intime() とは返値の真偽が逆である。
また真偽値が変化するのは一度だけである。
（イベントがすでに終了していれば偽を返す）

```c
// Arduinoスケッチで言えば
void setup (void) {
  event.setTimelimit(1000);
}
void loop (void) {
  if (event.timeup()) return;   // Time up (One Shot)
  // event.yield();
}
```

### bool intime (void)

setTimelimit() で設定した時間に達していなければ真を返す。
timeup() とは返値の真偽が逆である。
また真偽値が変化するのは一度だけである。
（イベントがすでに終了していれば真を返す）

```c
event.setTimelimit(1000);
while (event.intime()) {
  event.yield();              // 1000ms過ぎるまで何度でも実行される
}
```

----

## memstat.h

メモリ情報

依存性：
<stdlib.h>

### size\_t memFreeSize (void)

RAM領域の空きサイズを返す。
返値精度が 16bit幅か 32bit幅かは MCU品種に依存する。

```c
// #include <memstat.h>

size_t freeSize = memFreeSize();
```

この関数は現在のヒープ領域先頭とスタックポインタとの差を返す。
ヒープ確保に失敗したなら 0を返す。
ヒープ領域は 32byteアライメントに丸められるのでその程度の誤差は常に発生する。
この値は setup() 中で用いてすら、Arduino IDE が提示するより 40byte程度は少ないだろう。

----

## pcintvect.h

ピン変化割込

依存性：

注意：
SoftwareSerial とは併用できない。

### void attachPCInterrupt (uint8\_t interruptPin, void (*userFunc)(void) = NULL)

指定のピンに対応する外部ピン変化割込を有効にし、
指定のユーザー定義ルーチンを割込ベクタに割り付ける。

通常の attachInterrupt() と違って、第1引数にはピン番号を直接記述する。
また割込種別は EDGE しかないため、第3引数はない。
第2引数の割込ベクタは同一ポートグループに属する各最大8本のピンで共有され、
個々のピンに個別に割り当てるものではないことに注意。

MCU休止状態の解除のように、割込発生の事実だけを利用して割込処理が不要であるなら、第2引数には NULLを指定する。

割込ベクタ（ポートグループ）は PCINT0からPCINT3の最大4つがあるが、
MCUの品種によってピン割付定義が異なる。
また Arduino の表向きのピン番号とも一致しない。

|MCU|PCINT0|PCINT1|PCINT2|PCINT3|品種|
|---|---|---|---|---|---|
|ATmega328P|8 9 10 11 12 13|A0 A1 A2 A3 A4 A5 (A6 A7)|0 1 2 3 4 5 6 7|---|UNO ProMini|
|ATmega32U4|8 9 10 11 (14 15 16 17)|---|---|---|Leonardo ProMicro|
|ATmega1284P|A0 A1 A2 A3 A4 A5 A6 A7|4 5 6 7 10 11 12 13|22 23 24 25 26 27 28 29|0 1 2 3 8 9 30 31|Boubino|

いくつかのピンは、attachInterrupt() での割込指定と独立して共存する。
あるいは HardwareSerial や SPI や I2C の割込とも独立して共存する。
つまりそれらの専用機能割込とは全く別に、干渉せずに使用できる。

例えば HardwareSerial は MCUが休止中は割込を発生しないが、
外部ピン変化割込は検出され、休止状態を解除させることができる。
両方を併用することで MCUを適切な省電力状態に置くことができる。
個々の割込でのピン状態（HIGH・LOW、あるいは立ち上がり・立ち下がり変化）はソフトウェアで判断しなければできないが、
休止状態解除目的に限ればそういった処理は省略可能である。

ポートグループ内のどのピンが割込発生要因になったかを判断するのは難しい。
だが異なるポートグループの単一のピンに限って使用するのであれば、
そのピンのHIGH・LOW変化だけを調べれば良いので単純であるし、
普通はそのようにして使うだろう。
SoftwareSerial はまさにそのようにして外部ピン変化割込を利用している。

### void detachPCInterrupt (uint8\_t interruptPin)

指定のピンに対応するPCINT割込を解除する。
対応するポートグループの割込ベクタも解除される。

同一ポートグループに複数の attachPCInterrupt() 割り付けをしていた場合、
重複管理は行っていないので、まだほかで使われているにもかかわらず、
ポートグループベクタそのものが消されてしまう。
つまり detachPCInterrupt() の多用を前提にするなら、重複管理を行うか、
元からポートグループが共有されないように割込用の物理ピン配線を熟考すべきだろう。
Arduino.h で外部ピン変化割込が標準実装されていないのは、この分かり難さによるだろう。

----

## 既知の不具合／制約／課題

- 主要な AVR 以外はテストされていない。
- 古い Arduino IDE には対応しない。1.8.5で動作確認。少なくとも C++11 は使えなければならない。
- 英文マニュアルが未整備である。

## 改版履歴

- 0.1.2
  - bcdtime.h を追加。
  - digitalToggle() を雑用マクロに変更。
  - IntervalEvent.h の仕様を変更

## 使用許諾

MIT

## 著作表示

朝日薫 / askn
(SenseWay Inc.)
Twitter: [@askn37](https://twitter.com/askn37)
GitHub: https://github.com/askn37
