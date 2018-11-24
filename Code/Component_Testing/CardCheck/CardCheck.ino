#include <SD.h> // SD Card Library
#include <SPI.h> // SPI Communication 
#include <MFRC522.h> // RFID Card 

File Files;
char cardBuf[70];
char cards[5][11];
String Card0 = "";
String Card1 = "";
String Card2 = "";
String Card3 = "";
String Card4 = "";

int successfulRead = 0;
String enteredCard = "";

#define CS_RFID     22
#define RST_PIN     23
#define CS_SD_CARD  25
MFRC522 mfrc522(CS_RFID, RST_PIN);

#define ENTER   47
void setup() {
  Serial.begin(9600); 
  pinMode(ENTER, INPUT_PULLUP); 
  while(!Serial);

  SPI.begin();
  digitalWrite(CS_RFID, LOW);
  digitalWrite(CS_SD_CARD, HIGH);
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  delay(1000);
  digitalWrite(CS_RFID, HIGH);
  digitalWrite(CS_SD_CARD, LOW);
  if (SD.begin(CS_SD_CARD)){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("good");
    fillCardBuffer();
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("no begin");
  }
}

void loop() {
  if (!digitalRead(ENTER)){
    Serial.println("Scanning Card");
    getID();
    //Serial.println(enteredCard);
    delay(500);

    for (int i = 0; i <= 10; i++){
      Card0.concat(cards[0][i]);
    }
    Serial.print(enteredCard);
    Serial.print(" is ");
    if (enteredCard == Card0)
      Serial.print("Equal");
    else
      Serial.print("Not Equal");

    Serial.print(" to ");
    Serial.println(Card0);


  }
}

/*void CardCheck(String enteredCard, ){
  
}*/

void fillCardBuffer() { // Gets all the cards from Sd Card and places them in Card Buffer
  int index = 0;
  int cTotal = 0;
  int cIndex = 0;
  int nextCard = 0;
  
  Files = SD.open("Cards.txt", FILE_READ);

  if (Files)
  {
    Serial.println("Card.txt exist");
    while (Files.available()){
      cardBuf[index] = Files.read();
      index++;
    }
    Files.close();

    
    // Prints card buffer
    for (int i = 0; i < 70; i++){
        Serial.print(i); Serial.print(","); Serial.println(cardBuf[i]);
    }

    
    // Organizes the cards 
    for (int i = 0; i < 70; i++){
      
      if ( (i == 11) || (i == 12) || (i == 13) ||  
           (i == 25) || (i == 26) || (i == 27) ||
           (i == 39) || (i == 40) || (i == 41) ||
           (i == 53) || (i == 54) || (i == 55) ||
           (i == 67) /*|| (i == 64) || (i == 65)*/) 
      {
        // Dont Save
      }
      
      if ( (i == 14) || (i == 28) || (i == 42) || (i == 56) )
      { // Reset Card index before saving
        cIndex = 0;
        nextCard = 1;

        if (nextCard == 1)
           cTotal++;

        /*switch (cTotal){
            case 0:
              Card0.concat(cardBuf[i]);
              break;
            case 1:
              Card1.concat(cardBuf[i]);
              break;
            case 2:
              Card2.concat(cardBuf[i]);
              break;
            case 3:
              Card3.concat(cardBuf[i]);
              break;
            case 4:
              Card4.concat(cardBuf[i]);
              break;
        }*/
        
         cards[cTotal][cIndex] = cardBuf[i]; // Saves char in a more organized array
         cIndex++;
         nextCard = 0;
      }
      else
      {
        //if (nextCard == 1)
           //cTotal++;
        /*
        switch (cTotal){
            case 0:
              Card0.concat(cardBuf[i]);
              break;
            case 1:
              Card1.concat(cardBuf[i]);
              break;
            case 2:
              Card2.concat(cardBuf[i]);
              break;
            case 3:
              Card3.concat(cardBuf[i]);
              break;
            case 4:
              Card4.concat(cardBuf[i]);
              break;
        }*/
        cards[cTotal][cIndex] = cardBuf[i]; // Saves char in a more organized array
        cIndex++;
        nextCard = 0;
      }
    }

    
    // Prints saved cards
    /*for (int i = 0; i < 5; i++){
      for (int j = 0; j < 11; j++){
        Serial.print(i); Serial.print("."); Serial.print(j); Serial.print(" - "); Serial.println(cards[i][j]);
      }
    }*/
    /*Serial.println(Card0);
    Serial.println(Card1);
    Serial.println(Card2);
    Serial.println(Card3);
    Serial.println(Card4);*/
    
  }
  else
  {
    Serial.println("SD Card reader failed to initialize");
  }
}

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
  // Until we sLEFTport 7 byte PICCs
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

