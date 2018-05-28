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
#include <halt.h>
#include <memstat.h>
#include <adcomp.h>
#include <pcintvect.h>
#include <gpio.h>
#include <IntervalEvent.h>
```

# リファレンス

## halt.h

WDT_vect 応用ツール集。

依存性：
<avr/wdt.h>
<avr/sleep.h>

注意：
Leonardo（ATmega32U4）等の USB-UART内臓型では期待したとおりには動作しない。

### uint16\_t halt (uint16\_t SECONDS = 0, uint8_t MODE = SLEEP_MODE_PWR_DOWN)

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

## memstat.h

メモリ管理関係

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

## adcomp.h

AD比較器関係

依存性：

### uint16\_t getVcc (void)

AVCC入力電圧を測定して返す。
返値は無符号整数で、精度は 1/1000 Volt の固定小数点3桁である。

```c
// #include <adcomp.h>

uint16_t vcc = getVcc();
Serial.print(vcc / 1000.0);
```

### uint16\_t getTemperature (void)

一部の MCUが持つ内臓温度計の測定値を返す。
（未実装）

## pcintvect.h

PCINT割込補助

依存性：

注意：
SoftwareSerial とは併用できない。

### void attachPCInterrupt (uint8_t interruptPin, void (*userFunc)(void) = NULL)

指定のピンに対応する外部ピン変化割込を有効にし、
指定のユーザー定義ルーチンを割込ベクタに割り付ける。

通常の attachInterrupt() と違って、第1引数にはピン番号を直接記述する。
また割込種別は EDGE しかないため、第3引数はない。
第2引数の割込ベクタは同一ポートグループに属する各最大8本のピンで共有され、
個々のピンに個別に割り当てるものではないことに注意すること。

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
休止状態解除目的に限ればそういった処理は省略可能である。

ポートグループ内のどのピンが割込発生要因になったかを判断するのは難しい。
だが異なるポートグループの単一のピンに限って使用するのであれば、
そのピンのHIGH・LOW変化だけを調べれば良いので単純であるし、
普通はそのようにして使うだろう。
（SoftwareSerial はまさにそのようにして外部ピン変化割込を利用している）

### void detachPCInterrupt (uint8_t interruptPin)

指定のピンに対応するPCINT割込を解除する。
対応するポートグループの割込ベクタも解除される。

## gpio.h

GPIO関係

依存性：

### void digitalToggle (uint8_t outputPin)

指定のピンの出力をトグル変化する。
ピンが OUTPUT に指定されているなら HIGH と LOW を、
INPUT に指定されているならプルアップ抵抗の ON と OFF を切り替える。

## hexdigit.h

16進文字変換

依存性：

### uint8_t dtoh (const uint8_t dint)

引数の下位 4bitに対応する ASCII文字を返す。

### uint8_t htod (const uint8_t hchar)

16進数を表す ASCII文字の引数に対応する 4bitの正数を返す。

## bitsconv.h

表示用途ビット変換

依存性：
<avr/pgmspace.h>

### uint16_t wbits (const uint8_t bits)

8bitの引数に対応する 16bitの "倍角" ビットパターンを返す。

### uint8_t rbits (const uint8_t bits)

8bitの引数に対応する 8bitの "鏡対称" ビットパターンを返す。

## IntervalEvent.h

タイマー非依存定期実行イベントクラス

依存性：
<stdlib.h>

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
登録イベント数には空きメモリがある限り上限はない。

### IntervalEvent (void)

コンストラクタ。

### int setInterval (void (*userFunc)(void), uint32_t interval)

intervalミリ秒間隔で定期実行するイベントを作成し、成功ならイベントIDを返す。

### int setTimeout (void (*userFunc)(void), uint32_t interval)

intervalミリ秒後に一度だけ実行するイベントを作成し、成功ならイベントIDを返す。

### bool clear (int eventId)

イベントIDを指定し、そのイベントを取り消す。
イベントが存在しなければ偽を返す。

### bool yield (int eventId = 0)

所定の時間を経過していたらイベントを実行する。
引数にイベントIDを指定した場合は、そのイベントが実行された場合に真を返す。

## 既知の不具合／制約／課題

- 主要な AVR 以外はテストされていない。
- 古い Arduino IDE には対応しない。1.8.5で動作確認。少なくとも C++11 は使えなければならない。
- 英文マニュアルが未整備である。

## 改版履歴

- 0.1.1

## 使用許諾

MIT

## 著作表示

朝日薫 / askn
(SenseWay Inc.)
Twitter: [@askn37](https://twitter.com/askn37)
GitHub: https://github.com/askn37
