# IR Bug Can With RS485 Modbus RTU

## 說明
本專案承接於 [ChiShengChen/pure_agri_bugcan_pest-counter](https://github.com/ChiShengChen/pure_agri_bugcan_pest-counter)
<br/><br/>
這個Repository的目標是使用RS485通訊標準，並且使用Modbus RTU作為通訊協定。目標是讓田間系統可以向蟲罐讀取數值。因此，蟲罐蟲罐計數害蟲通過的程式碼在這裡僅以簡單的按鈕取代(並不是正式版本)，本說明也將注重在通訊的部分。<br/><br/>
關於之前做錯的程式碼與說明仍可以在這個Repository的[master](https://github.com/twbrandon7/IR-Bug-Can/tree/master) branch看到。

## 規格
|項目|數值|
|--|--|
|RS485電源|3.3V|
|通訊協定|Modbus RTU|
|Bitrate|9600 bps|
|Device ID|3|
|Register總長度|5|

<br/>

### Register Address
|Address|名稱|Function Code|型態|說明|
|--|--|--|--|--|
|0000H|HOURS|03H|int|從上次連線開始到這次連線開始所經過的小時數|
|0001H|MINUTES|03H|int|從上次連線開始到這次連線開始所經過的分鐘數|
|0002H|SECONDS|03H|int|從上次連線開始到這次連線開始所經過的秒數|
|0003H|BUG_PEST|03H|int|從上次連線開始到這次連線開始所累積的害蟲數|
|0004H|END|03H|int|資料結尾，固定為-1|

<br/>

## TODO List
- 實際串接上蟲罐
- 改寫RS485 Modbus library，讓其支援限制對Register的讀寫

<br/>

## 使用到的RS485 Modbus library
[https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino](https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino)

<br/>

## 測試時所使用的接線
在這裡稍微紀錄一下我開發/測試/學習的經過。

<br/>

### 接線
#### The Wiring of Slave and RS485 Module (1)
|Arduino Nano|RS485 Module|
|--|--|
|3.3V|VCC|
|GND|GND|
|R3|DE and RE|
|D10|RO|
|D11|DI|

<br/>

#### The Wiring of Slave and simulated bug can
|Arduino Nano|Bug Can (just a button...)|
|--|--|
|pin7 (Input)|Push Button|

<br/>

![RS485 Modbus RTU](https://lh3.googleusercontent.com/EZoe38OZlClwqqRVyda3E6jxNoF2i8D_2Ek4QqCO5r9T5XDlKpI9Wd0amvubdxM2S5cJ3SDLCZ_b)

<br/>

## 參考資料
- [SoftwareSerialRS485Example](https://arduino-info.wikispaces.com/SoftwareSerialRS485Example)
- [modbus?關於modbus RTU的使用說明](http://www.xuan.idv.tw/wordpress/?p=1705)
- [modbus?關於modbus RTU的使用說明II](http://www.xuan.idv.tw/wordpress/?p=2427)
- [Modbus - Wikipedia](https://en.wikipedia.org/wiki/Modbus)

<br/>

## 後記
我測試的程式碼可以在這裡找到 : 
- Slave (蟲罐端) : [RS485_Slave](https://github.com/twbrandon7/IR-Bug-Can/tree/modbus/RS485_Slave "RS485_Slave")
- Master (模擬田間系統端) : 使用PC模擬，軟體 : [Modbus Master Simulator](http://en.radzio.dxp.pl/modbus-master-simulator/)

<br/>

2018/07/21 Li-Xian Chen @ NIU CSIE<br/>
b0543017@ems.niu.edu.tw