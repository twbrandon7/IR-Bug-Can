# IR Bug Can With RS485 Module

## 說明
本專案承接於 [ChiShengChen/pure_agri_bugcan_pest-counter](https://github.com/ChiShengChen/pure_agri_bugcan_pest-counter)

這個Repository的目標是使用RS485通訊標準，來讓智慧蟲罐可以定時將資料傳送到Arm Based Arduino。因此，蟲罐蟲罐計數害蟲通過的程式碼在這裡僅以簡單的按鈕取代，本說明也將注重在通訊的部分。

## 硬體設備
這裡將列出我測試時所用的硬體與接線。

### 硬體總表
|名稱|數量|
|--|--|
| Arduino Uno | 1 |
|Arduino Nano|1|
| [RS485通訊模組](https://www.taiwaniot.com.tw/product/ttl-to-rs485rs422-max485-module/) | 2 |

### 接線
#### The Wiring of Slave and RS485 Module (1)
|Arduino Nano|RS485 Module (1)|
|--|--|
|5V|VCC|
|GND|GND|
|R3|DE and RE|
|D10|RO|
|D11|DI|

#### The Wiring of RS485 Module (1) and RS485 Module (2)
|RS485 Module (1)|RS485 Module (2)|
|--|--|
|A|A|
|B|B|

#### The Wiring of RS485 Module (1) and Master
|RS485 Module (2)|Arduino Uno R3 (for test only)|
|--|--|
|VCC|5V|
|GND|GND|
|DE and RE|pin 3|
|RO|pin 10|
|DI|pin 11|

![接線概觀](https://lh3.googleusercontent.com/w3lIfGIho3Mc1TEgZTIB1WhlJHfyxhJkMExcUSowExYsqddjVDjsO081lMPq8Q5nLVhAptXBfTuk)

接線主要是要確定Arduino作為TX的pin要接到RS485模組的DI，RX的pin要接上RS485模組的RO。兩個RS485模組之間A接A、B接B。另外模組上的DE與RE要一同接上Arduino的同一個pin作為TxControll，當TxControll是HIGH的時候，代表送資料，LOW代表收資料。

## 韌體
以目前的進度，我暫時使用Arduino的[SoftwareSerial](https://www.arduino.cc/en/Reference/SoftwareSerial) Library來實現通訊的功能。
我已經將整個韌體打包成一個Arduino的函式庫，只要直接下載、引入標頭檔之後就能使用。詳細使用方式將在後面說明。

### Slave (蟲罐) 端
蟲罐端將可以決定多少毫秒後送出一次害蟲數量。送出資訊時將以以下順序送出 : 
![蟲罐資料輸出](https://lh3.googleusercontent.com/cy8HoS2pK47kDompA8BbLZ4BXPFLdUr-ZWHeAMTuj8cLEg0tTgzGseP7Eqk0cVpvOdfG3IHk7_F0 "蟲罐資料輸出")

資料送出格式中 :
- 序列開頭 : 固定是 -1，表示序列開始
- 累積時間 : 上次資料發送到這次資料發送所經過的分鐘數
- 累積次數 : 上次資料發送到這次資料發送的害蟲數量
- 序列結尾 : 固定是 -2，表示序列結束

實際送出時，會將int轉換為4位的16進制字串，並且依序傳送ASCII Code出去。

### Master (田間系統) 端
我在測試的時候，使用Arduino Uno R3來做測試。
接收資料時會接收到0~255的數值，在這邊是接收Slave傳來的ASCII Code，我將每個收到的ASCII Code轉換為字元，每4個字元組成一個字串，再轉回int。
![田間系統資料接收](https://lh3.googleusercontent.com/hOzjWgGFN0IYeuVbQSO7zA1c0wWBK3XvypqgXgS5NhH_hnhRtuouFcDC57lMC3I9iaEJMGSGRYfs "田間系統資料接收")

這個是目前的傳輸方式，未來我可能不考慮繼續修改，而是直接使用ModBus通訊協定。

## IR Bug Can Lib
我已經將上述這些通訊方式打包成Arduino的Library，取名為IR Bug Can Lib。只要下載並且引入標頭檔就能使用。

在使用以下說明以前，要先安裝好Library。
網址 : [https://github.com/twbrandon7/IR-Bug-Can/tree/master/IRBugCanLib](https://github.com/twbrandon7/IR-Bug-Can/tree/master/IRBugCanLib)

### Slave (蟲罐)端使用方式
```cpp
//先引入標頭檔
#include <IrBugCanLib.h>

/* 建構出SoftwareSerial物件，建構子的地方要填入兩個參數
 * PinRX : 要作為RX的pin，這個pin將會連接RS485模組的RO
 * PinTX : 要作為TX的pin，這個pin將會連接RS485模組的DI
 */
SoftwareSerial RS485Serial(PinRX, PinTX); // RX, TX

/* 建構出IrBugCanLib物件，建構子的地方要填入以下參數
 * &RS485Serial : SoftwareSerial物件的指標
 * PinTXControl : 要作為TXControl的pin，會連接RS485模組的DE和RE
 * PinTransmissionLED : 傳輸提示LED，傳輸資料時將會閃爍。設定為-1表示不使用。
 * CommunicationBaudRate : 傳輸資料使用的Baud rate，Master與Slave必須設定為相同的數值。
 */
IrBugCanLib irBugCanLib(&RS485Serial, PinTXControl, PinTransmissionLED, CommunicationBaudRate);

void setup() {
  //記得在setup()裡面呼叫，作為初始化
  irBugCanLib.begin();
}

void loop() {
  /*
   * 送出資料時可以調用snedData()
   * actualDuration : (int) 經過時間
   * bugPestCount : (int) 害蟲數量
   * */
  irBugCanLib.snedData(actualDuration, bugPestCount);
  delay(5000);
}
```

### Master (田間系)端
```cpp
//先引入標頭檔
#include <IrBugCanLib.h>

/* 建構出SoftwareSerial物件，建構子的地方要填入兩個參數
 * PinRX : 要作為RX的pin，這個pin將會連接RS485模組的RO
 * PinTX : 要作為TX的pin，這個pin將會連接RS485模組的DI
 */
SoftwareSerial RS485Serial(PinRX, PinTX); // RX, TX

/* 建構出IrBugCanLib物件，建構子的地方要填入以下參數
 * &RS485Serial : SoftwareSerial物件的指標
 * PinTXControl : 要作為TXControl的pin，會連接RS485模組的DE和RE
 * PinTransmissionLED : 傳輸提示LED，傳輸資料時將會閃爍。設定為-1表示不使用。
 * CommunicationBaudRate : 傳輸資料使用的Baud rate，Master與Slave必須設定為相同的數值。
 */
IrBugCanLib irBugCanLib(&RS485Serial, PinTXControl, PinTransmissionLED, CommunicationBaudRate);

void setup() {
  //記得在setup()裡面呼叫，作為初始化
  irBugCanLib.begin();
}

void loop() {
  //接收資料的部分寫在這
  IrBugCanData data; //先宣告出一個struct來收資料
  irBugCanLib.readData(data); // 將struct傳入，交由readData()這個method來寫入資料。
  if(data.has_data) // 如果有收資料的話
  { 
	//印出經過時間
	Serial.println("Time Pass : " + String(data.time_pass));
	//印出害蟲數量
	Serial.println("Bug Pest : " + String(data.bug_pest));
  }
}
```

## 田間系統要怎麼串接上蟲罐?
在這裡做個總整理。

### 硬體
因為Arduino並不能直接使用RS485，因此需要額外連接RS485模組。所需硬體清單如下 : 
|名稱|數量|
|--|--|
| [RS485通訊模組](https://www.taiwaniot.com.tw/product/ttl-to-rs485rs422-max485-module/) | 1 |

### 接線
選定3個pin分別作為TX、RX、TXControl。之後蟲罐就只會有兩個pin對外連接(即RS485 Module 的 A 和 B)。

#### The Wiring of RS485 Module and Master
|RS485 Module (2)|Arduino Uno R3 (for test only)|
|--|--|
|VCC|5V|
|GND|GND|
|DE and RE|TXControl|
|RO|RX|
|DI|TX|

#### The Wiring of RS485 Module and IR Bug Can
|RS485 Module|RS485 Module (IR Bug Can)|
|--|--|
|A|A|
|B|B|

### 韌體
請先下載並安裝[IR Bug Can Lib](https://github.com/twbrandon7/IR-Bug-Can/tree/master/IRBugCanLib)。使用方式如下 : 

```cpp
//先引入標頭檔
#include <IrBugCanLib.h>

/* 建構出SoftwareSerial物件，建構子的地方要填入兩個參數
 * PinRX : 要作為RX的pin，這個pin將會連接RS485模組的RO
 * PinTX : 要作為TX的pin，這個pin將會連接RS485模組的DI
 */
SoftwareSerial RS485Serial(PinRX, PinTX); // RX, TX

/* 建構出IrBugCanLib物件，建構子的地方要填入以下參數
 * &RS485Serial : SoftwareSerial物件的指標
 * PinTXControl : 要作為TXControl的pin，會連接RS485模組的DE和RE
 * PinTransmissionLED : 傳輸提示LED，傳輸資料時將會閃爍。設定為-1表示不使用。
 * CommunicationBaudRate : 傳輸資料使用的Baud rate，Master與Slave必須設定為相同的數值。
 */
IrBugCanLib irBugCanLib(&RS485Serial, PinTXControl, PinTransmissionLED, CommunicationBaudRate);

void setup() {
  //記得在setup()裡面呼叫，作為初始化
  irBugCanLib.begin();
}

void loop() {
  //接收資料的部分寫在這
  IrBugCanData data; //先宣告出一個struct來收資料
  irBugCanLib.readData(data); // 將struct傳入，交由readData()這個method來寫入資料。
  if(data.has_data) // 如果有收資料的話
  { 
	//印出經過時間
	Serial.println("Time Pass : " + String(data.time_pass));
	//印出害蟲數量
	Serial.println("Bug Pest : " + String(data.bug_pest));
  }
}
```
	
## TODO List
- 直接使用ModBus傳輸協定
- 添加Slave端的程式說明
- 實際串接上蟲罐
- 添加IR Bug Can Lib的說明

## 參考資料
- [SoftwareSerialRS485Example](https://arduino-info.wikispaces.com/SoftwareSerialRS485Example)
- [Arduino-Hex-Decimal-Conversion](https://github.com/benrugg/Arduino-Hex-Decimal-Conversion/blob/master/hex_dec.ino)

## 後記
我測試的程式碼可以在這裡找到 : 
- Slave (蟲罐端) : [RS485_Slave](https://github.com/twbrandon7/IR-Bug-Can/tree/master/RS485_Slave "RS485_Slave")
- Master (模擬田間系統端) : [RS485_Master](https://github.com/twbrandon7/IR-Bug-Can/tree/master/RS485_Master "RS485_Master")

2018/07/20 Li-Xian Chen @ NIU CSIE
b0543017@ems.niu.edu.tw