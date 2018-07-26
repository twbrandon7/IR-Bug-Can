# IR Bug Can With RS485 Modbus RTU

## 說明
本專案承接於 [ChiShengChen/pure_agri_bugcan_pest-counter](https://github.com/ChiShengChen/pure_agri_bugcan_pest-counter)
<br/><br/>
這個Repository的目標是使用RS485通訊標準，並且使用Modbus RTU作為通訊協定，將蟲罐的資料傳輸給田間系統。目前已經完成所有的設置與連接。<br/><br/>
關於之前沒有使用Modbus的版本，程式碼與說明仍可以在這個Repository的[master](https://github.com/twbrandon7/IR-Bug-Can/tree/master) branch看到。

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

## 機制
在這裡簡略說明蟲罐的機制。<br/>
蟲罐是一個Slave，會等待Master對其建立連線。在這邊我將連線的定義設定為 : Master開始對Slave Polling，兩次polling的時間間隔如果小於1500ms，則視為同一次連線，否則視為Timeout，連線中斷。<br/>
- 每當一個連線開始 : 
 - 蟲罐會回傳一組固定的數值不再變動，並且將計數歸零，在背景重新開始累積。
- 每當一個連線結束 : 
 - 不做任何動作。

因此，Master建立連線後取得的數值將會是固定的。而Master將會取得以下資訊 : 
- 從上次連線開始到這次連線開始所經過的時間(小時)
- 從上次連線開始到這次連線開始所經過的時間(分鐘)
- 從上次連線開始到這次連線開始所經過的時間(秒)
- 從上次連線開始到這次連線開始所累積的害蟲數
(詳見表 "Register Address")<br/>
在應用上，Master可以藉由建立連線的時間減去經過的時間，得到一個精準的累計時間間隔。

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

#### The Wiring of Slave and IR Bugcan
|Arduino Nano|Bugcan|
|--|--|
|D4 (Input)|White wire|
|5V|Red wire|
|GND|Black wire|

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

2018/07/25 Li-Xian Chen @ NIU CSIE<br/>
b0543017@ems.niu.edu.tw