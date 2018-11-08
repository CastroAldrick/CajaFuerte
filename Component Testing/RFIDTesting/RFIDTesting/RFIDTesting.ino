/*
 * Programmer: Aldrick
 * Description: Testing of the RFID Scanner
 * Wiring: 
 * 
 * RFID Scanner -> Arduino Micro
 *          RST -> 9
 *      SDA(CS/SS) -> 10
 *         MOSI -> MOSI
 *         MISO -> MISO
 *          SCK -> SCK
 *          VCC -> 3.3V
 *          GND -> GND
 */
 // Includes
 #include <EEPROM.h>
 #include <SPI.h>
 #include <MFRC522.h>

// Create MFRC522 instance.
 #define SS_PIN 22
 #define RST_PIN 23
 MFRC522 mfrc522(SS_PIN, RST_PIN);

  int successfulRead = 0;
  String enteredCard = "";
  
void setup() {
  //Initializations
  pinMode(25 ,OUTPUT);
  digitalWrite(25 ,HIGH);
  
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  while(!Serial);
  Serial.println("Starting");
}

void loop() {
  // Wait for card to be scan
  successfulRead = getID();
  //Serial.println(successfulRead);

  if (successfulRead == 1){
    Serial.println(enteredCard);
    Serial.println("Done");

    //Conditions
    if (enteredCard == "DE 26 7D 63"){
      Serial.println("Valid");
    }
    successfulRead = 0;
    enteredCard = "";
  }
}

 // Function to read whats on the card
 uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  Serial.println(F("Scanned PICC's UID:"));
  for ( byte i = 0; i < mfrc522.uid.size; i++) {
    if (i != 0){
      enteredCard.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    }
    enteredCard.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  enteredCard.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

/*  Modification Date/Description
 * 
 * 09/03/2018 ac  Creating a program to test the RFID Scanner
 */
 
