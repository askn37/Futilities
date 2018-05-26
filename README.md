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
#include <Futilities.h>

// あるいは必要な部分だけ
#include <halt.h>
```

## halt.h

WDT_vect 応用ツール集。

依存性：
<avr/wdt.h>
<avr/sleep.h>

注意：
UNO（ATmega328P）および Boubino（ATmega1284P）等向けである。
Leonardo（ATmega32U4）等の USB-UART標準内臓系列では
コンパイルはできるものの期待したとおりには動作しない。
halt() は USB-UART を停止しないと正しく休止状態に入れない。
reboot() 後は USB-UART が正しく再初期化されない。etc.

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

### void wdtAttach (volatile void(*CALLBACK)(void))

WDT_vect にユーザ定義割込ルーチンをセットする。

```c
// #include <halt.h>

volatile uint16_t wdt_count = 0;
volatile void ISR_wdt (void) {
    wdt_count++;
}
wdtAttach(ISR_wdt);
wdtStart(WDTO_1S);
```

halt() 実行中以外は WDT_vect タイマーは使用されていないため、ユーザレベルで使用しても構わない。
このエントリは（halt.hで定義された）真の WDT_vect から call される。
ただし halt() を実行するたびにこの指定は上書きされて失われるので、
wdtAttach() は逐次実行する必要がある。

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

# 既知の不具合／制約／課題

- 主要な AVR 以外はテストされていない。
- 古い Arduino IDE には対応しない。1.8.5で動作確認。少なくとも C++11 は使えなければならない。
- 英文マニュアルが未整備である。

# 改版履歴

- 0.1.0

# 使用許諾

MIT

# 著作表示

朝日薫 / askn
(SenseWay Inc.)
Twitter: [@askn37](https://twitter.com/askn37)
GitHub: https://github.com/askn37
