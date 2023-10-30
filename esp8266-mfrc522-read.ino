#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D1
#define SS_PIN D2

MFRC522 mfrc522(SS_PIN, RST_PIN);

#define NR_KNOWN_KEYS   16
// Known keys, see: https://github.com/XaviTorello/mifare-classic-toolkit/blob/master/rfid_defaultKeys_tester.ino
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 00 00 00 00 00 00
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // d3 f7 d3 f7 d3 f7 
    {0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0}, // a0 b0 c0 d0 e0 f0
    {0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1}, // a1 b1 c1 d1 e1 f1
    {0x71, 0x4c, 0x5c, 0x88, 0x6e, 0x97}, // 71 4c 5c 88 6e 97
    {0x58, 0x7e, 0xe5, 0xf9, 0x35, 0x0f}, // 58 7e e5 f9 35 0f
    {0xa0, 0x47, 0x8c, 0xc3, 0x90, 0x91}, // a0 47 8c c3 90 91
    {0x53, 0x3c, 0xb6, 0xc7, 0x23, 0xf6}, // 53 3c b6 c7 23 f6
    {0x8f, 0xd0, 0xa4, 0xf2, 0x56, 0xe9}  // 8f d0 a4 f2 56 e9
};

MFRC522::StatusCode status;
byte buffer[18];
byte waarde[64][16];

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("\nReady");
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void dump_byte_array1(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.write(buffer[i]);
  }
}

bool try_key(MFRC522::MIFARE_Key *key)
{
    bool result = false;
    
    for(byte block = 0; block < 64; block++){
      
    // Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print("PCD_Authenticate() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Read block
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (status != MFRC522::STATUS_OK) {
        Serial.print("MIFARE_Read() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else {
        // Successful read
        result = true;
        Serial.print("Success with key:");
        dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
        Serial.println();
        
        // Dump block data
        Serial.print("Block "); Serial.print(block); Serial.print(":");
        dump_byte_array1(buffer, 16); //omzetten van hex naar ASCI
        Serial.println();
        
        for (int p = 0; p < 16; p++) //De 16 bits uit de block uitlezen
        {
          waarde [block][p] = buffer[p];
          Serial.print(waarde[block][p]);
          Serial.print(" ");
        }
        
        }
    }
    Serial.println();
    
    //Serial.println("1.Read card \n2.Write to card \n3.Copy the data.");

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
    
    //start();
}

void loop () {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      String uid_;
      uid_ += String(mfrc522.uid.uidByte[1]) + String(mfrc522.uid.uidByte[2]) + String(mfrc522.uid.uidByte[3]) + String(mfrc522.uid.uidByte[4]);
      Serial.print("Card UID: "); 
      Serial.println(uid_.toInt());
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
      Serial.println();
      Serial.print("PICC type: ");
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));
      delay(10);
      // Try the known default keys
      MFRC522::MIFARE_Key key;
      for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
        // Copy the known key into the MIFARE_Key structure
        for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
            key.keyByte[i] = knownKeys[k][i];
        }
        // Try the key
        if (try_key(&key)) {
            // Found and reported on the key and block,
            // no need to try other keys for this PICC
            break;
        }
    }
    }
}