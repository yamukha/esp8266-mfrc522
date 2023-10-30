# esp8266-mfrc522
RFID cloner by ESP8266 with  MFRC-522 (RC522)

# Used Links
https://www.youtube.com/watch?v=M5-T7HocAMc

https://github.com/miguelbalboa/rfid/blob/master/examples/RFID-Cloner/RFID-Cloner.ino
https://github.com/XaviTorello/mifare-classic-toolkit/blob/master/rfid_defaultKeys_tester.ino

https://alexgyver.ru/arduino-rfid/

# Wiring (ref. youtube video)
NodeMCU - MFRC522  (PIN) \
     D1 - RST  (7) \
     D2 - SDA  (1) \
     D5 - SCK  (2) \
     D6 - MISO (3) \
     D7 - MOSI (4) \
     G  - GND  (6) \
     3V - 3.3V (8)

# Usage
Serial port baud rate 115200\
Build under Arduino IDE i.e. version 1.8.20\
After download to NodeMCU:\
In Serial Monitor send 1\
Read RFID card\
In Serial Monitor send 2\
New data will be prepared to write blocks 
In Serial Monitor send 3\
Data will be cloned to RFID card\