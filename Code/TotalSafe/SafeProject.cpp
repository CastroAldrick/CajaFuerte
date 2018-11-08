 #include <SPI.h>
 #include <MFRC522.h>
 #include "SafeProject.h"
 #include <Adafruit_Fingerprint.h>

SafeProject::SafeProject(bool hi){
  if (hi == true)
    Serial.println("Object created\n");
}
    
byte SafeProject::getID() {
  MFRC522 mfrc522(SS_PINfp, RST_PIN);
  mfrc522.PCD_Init();
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
    return 0;
  if ( ! mfrc522.PICC_ReadCardSerial()) 
    return 0;
  for ( byte i = 0; i < mfrc522.uid.size; i++) {
    if (i != 0){
      enteredCard.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    }
    enteredCard.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  enteredCard.toUpperCase();
  mfrc522.PICC_HaltA();
  return 1;
}

void SafeProject::startFingerprintSensor(){
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor");
  }
  Serial.println("Waiting for valid finger...");
}

int SafeProject::getFingerprintID() {
  finger.begin(57600);
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
