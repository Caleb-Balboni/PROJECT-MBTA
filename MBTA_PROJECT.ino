 #include <Wire.h>
 #include <PN532_I2C.h>
 #include <PN532.h>
 #include <avr/wdt.h>
   PN532_I2C pn532i2c(Wire);
   PN532 nfc(pn532i2c);

void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}

String byteArrayToHexString(const uint8_t* data, size_t length) {
  String result;
  for (size_t i = 0; i < length; i++) {
    
    if (data[i] < 0x10) {
    
      result += '0';
    }
   
    result += String(data[i], HEX);
  }
  return result;
}

bool arrayEqual(uint8_t arr1[16], uint8_t arr2[16]) {
  for (int i = 0; i < 16; i++) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }
  return true;
}

void setup() {

  nfc.begin();
  Serial.begin(115200);
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  nfc.SAMConfig();
    Serial.println("Please tap card:");
}

void loop() {

 uint8_t keya[6] = { REDACTED };
 uint8_t dataToWrite[16] = { REDACTED }; 
 uint8_t dataToRead[16];
 uint8_t originalChecksum[2] = { REDACTED };
 uint8_t readData[16];    
 uint8_t uid[7];      // Buffer to hold the UID
 uint8_t uidLength;   // Length of the UID (usually 4 for Mifare Classic)

 if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
   byteArrayToHexString(uid, uidLength);
  if (nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 16, 1, keya)) {

  if (nfc.mifareclassic_ReadDataBlock(REDACTED, dataToRead)) {
    uint8_t checkSumMod1 = originalChecksum[0] ^ dataToRead[14];
    uint8_t checkSumMod2 = originalChecksum[1] ^ dataToRead[15];
    dataToWrite[14] = REDACTED ^ checkSumMod1;
    dataToWrite[15] = REDACTED ^ checkSumMod2;

   if (nfc.mifareclassic_AuthenticateBlock(uid, uidLength, REDACTED, 1, keya)) {
     if(nfc.mifareclassic_WriteDataBlock(REDACTED, dataToWrite)) {
        nfc.mifareclassic_ReadDataBlock(9, dataToRead);
        Serial.println("New Block REDCATED: " + byteArrayToHexString(dataToRead, REDACTED));
       if(nfc.mifareclassic_AuthenticateBlock(uid, uidLength, REDACTED, 1, keya) && arrayEqual(dataToWrite, dataToRead)) {
         if(nfc.mifareclassic_WriteDataBlock(REDACTED, dataToWrite)) {
          nfc.mifareclassic_ReadDataBlock(REDACTED, dataToRead);
            if (arrayEqual(dataToWrite, dataToRead)) {
                Serial.println("New Block REDACTED: " + byteArrayToHexString(dataToRead, REDACTED));
                Serial.print("Writing Success!");
                delay(5000);
                while(1) {}
             }
          }
        }
      }
    }
  }
}
Serial.println("Critical Error!");
delay(1000);
Serial.println("Resetting...");
delay(5000);
reboot();
}
}
