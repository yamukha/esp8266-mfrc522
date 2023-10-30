#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D1
#define SS_PIN D2

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(115200);         // Initialize serial communications with the PC
  delay(200);
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  Serial.println(F("\nWarning: this example clears your mifare UID, use with care!"));
  
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  if ( mfrc522.MIFARE_UnbrickUidSector(false) ) {
    Serial.println(F("Cleared sector 0, set UID to 1234. Card should be responsive again now."));
  }
  else {
     Serial.println(F("Failed to unbrick. Try again in 5s"));
  }
  delay(5000);
}