/**************************************
  Programmer: Aldrick
  Description: Arduino base Safe project using the Have/Are Authentication
  
                 Wiring
       Component        Arduino 
       
 **************************************/


/**************************************
 Includes      Explanation
 **************************************/
 
#include <TimerOne.h> // For Software interrLEFTts
#include <SD.h> // SD Card Library
#include <SPI.h> // SPI Communication 
#include <MFRC522.h> // RFID Card 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <Adafruit_Fingerprint.h>
#define mySerial Serial1
 int fingerprintID = 0;
 Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
 bool Done = false;
 uint8_t id;
DS3231 rtc(SDA, SCL);

// Navigation buttons
#define LEFT    46
#define RIGHT   48
#define BACK    49
#define ENTER   47

LiquidCrystal_I2C lcd(0x27,16,2);

unsigned int volatile beat = 10; // 1s
unsigned int volatile buttonDebounceTimer = 5; // 500ms
unsigned int volatile cardScanningTimer = 50; // 5s
unsigned int volatile fingerScanTimer = 50; // 5s

//Beating LED
#define BEATING_LED 24

File Files;
char cardBuf[70];
char cards[5][11];
String Card0 = "";
String Card1 = "";
String Card2 = "";
String Card3 = "";
String Card4 = "";
String sCards[5];

int hLocation = 1;
int menu = 0;
int vLocation = 0;
bool cleared = false;

int successfulRead = 0;
String enteredCard = "";
int resetCardTimer = 0;
int resetFingerScanningTimer = 0;

// Chip Selects for SPI Slaves
#define CS_RFID     22
#define RST_PIN     23
#define CS_SD_CARD  25
MFRC522 mfrc522(CS_RFID, RST_PIN);

void setup() { 
  Serial.begin(9600);  
  rtc.begin();
  finger.begin(57600);
  pinMode(BEATING_LED, OUTPUT);
  
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT , INPUT_PULLUP);
  pinMode(BACK, INPUT_PULLUP);
  pinMode(ENTER, INPUT_PULLUP);

  
  Timer1.initialize(100000); // 100ms interrupt rate
  Timer1.attachInterrupt( dec );

  lcd.init();
  lcd.backlight();
  
  

  while(!Serial);
  SPI.begin();
  digitalWrite(CS_RFID, LOW);
  digitalWrite(CS_SD_CARD, HIGH);
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();


  delay(1000);
  //SPI.begin();  
  digitalWrite(CS_RFID, HIGH);
  //digitalWrite(CS_SD_CARD, LOW);
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
  //SPI.end();
  //rtc.setTime(16, 41, 0);
}

void loop() { 
  jobs();
}

void jobs() { 
  // Indicates that the program is running
  if (!beat){
    digitalWrite(BEATING_LED, digitalRead(BEATING_LED) ^ 1);
    beat = 10;
  }

  // Runs function that controls the menu
  if(!buttonDebounceTimer){
    LCD_Map(menu);
  }
}

void dec() { 
  if (beat)
    beat--;
    
  if (buttonDebounceTimer)
    buttonDebounceTimer--;

  if (cardScanningTimer)
    cardScanningTimer--;

  if (fingerScanTimer)
    fingerScanTimer--;
}

void fillCardBuffer() { // Gets all the cards from Sd Card and places them in Card Buffer
  int index = 0;
  int cTotal = 0;
  int cIndex = 0;
  int nextCard = 0;
  
  Files = SD.open("Cards.txt", FILE_READ);

  if (Files)
  {
    Serial.println("Cards.txt exist");
    while (Files.available()){
      cardBuf[index] = Files.read();
      index++;
    }
    Files.close();

    digitalWrite(CS_SD_CARD, HIGH);
    
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

         cards[cTotal][cIndex] = cardBuf[i]; // Saves char in a more organized array
         cIndex++;
         nextCard = 0;
      }
      else
      {
        if (nextCard == 1)
           cTotal++;

         cards[cTotal][cIndex] = cardBuf[i]; // Saves char in a more organized array
         cIndex++;
         nextCard = 0;
      }
    }

    
    // Prints saved cards
    
    for (int i = 0; i < 5; i++){
      for (int j = 0; j < 11; j++){
        Serial.print(i); Serial.print("."); Serial.print(j); Serial.print(" - "); Serial.println(cards[i][j]);
      }
    }
  }
  else
  {
    Serial.println("SD Card reader failed to initialize");
  }

  //  Converts the Card arrays to Strings that can be used to compare
  cTotal = 0;
  cIndex = 0;
  nextCard = 0;
  
  for (int i = 0; i <= 10; i++){
    switch (cTotal){
      case 0:
        Card0.concat(cards[cTotal][i]);
        if (i == 10){
          i = -1;
          cTotal++;
        }          
        break;

      case 1:
        Card1.concat(cards[cTotal][i]);
        if (i == 10){
          i = -1;
          cTotal++;
        }          
        break;

      case 2:
        Card2.concat(cards[cTotal][i]);
        if (i == 10){
          i = -1;
          cTotal++;
        }          
        break;

      case 3:
        Card3.concat(cards[cTotal][i]);
        if (i == 10){
          i = -1;
          cTotal++;
        }          
        break;

      case 4:
        Card4.concat(cards[cTotal][i]);
        if (i == 10){
        }          
        break;
    }
  }
//  Serial.print("Card0 = "); Serial.println(Card0);
//  Serial.print("Card1 = "); Serial.println(Card1);
//  Serial.print("Card2 = "); Serial.println(Card2);
//  Serial.print("Card3 = "); Serial.println(Card3);
//  Serial.print("Card4 = "); Serial.println(Card4);
  //sCards[5] = {Card0, Card1, Card2, Card3, Card4};

  for (int x = 0; x <= 5; x++){
    switch (x){
      case 0:
        sCards[0] = Card0;
        break;
      case 1:
        sCards[1] = Card1;
        break;
      case 2:
        sCards[2] = Card2;
        break;
      case 3:
        sCards[3] = Card3;
        break;
      case 4:
        sCards[4] = Card4;
        break;
    }
  }

  //Serial.println(sCards[4]);
}

uint8_t getID(){ 
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

void updateCard(){
  SD.remove("CARDS.TXT");

  Files = SD.open("CARDS.TXT", FILE_WRITE);
  if (Files){
    Serial.println("Cards.txt openned");
    for (int i = 0; i <= 4; i++){
      Files.print(sCards[i]);
      Files.println(" ");
    }
    Serial.println("Done");
    Files.close();
  }
  Serial.println("Finished editting Cards.txt");
}

void rmAllCards(){
  SD.remove("CARDS.TXT");

  Files = SD.open("CARDS.TXT", FILE_WRITE);
  if(Files){
    Serial.println("Cards.txt openned");
    for (int i = 0; i <= 4; i++){
      Files.print(sCards[i]);
      Files.println(" ");
    }
    Serial.println("Done");
    Files.close();
  }
  Serial.println("Finished editting Cards.txt");
}

void startFingerprintSensor()
{
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor");
  }
  Serial.println("Waiting for valid finger...");
}

int getFingerprintID() {
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

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintEnroll(int id) {
  int p = -1;
  //Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  lcd.setCursor (0, 1);
  lcd.print("                ");
  lcd.setCursor (0, 1);
  lcd.print("Enter Finger");
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.setCursor (0, 1);
      lcd.print("                ");
      lcd.setCursor (0, 1);
      lcd.print("Image taken");
      delay(500);
      break;
      
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  lcd.setCursor (0, 1);
  lcd.print("                ");
  lcd.setCursor (0, 1);
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.setCursor (0, 1);
  lcd.print("                ");
  lcd.setCursor (0, 1);
  lcd.print("EntrFinger Again");
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.setCursor (0, 1);
    lcd.print("                ");
    lcd.setCursor (0, 1);
    lcd.print("Stored print# "); lcd.print(id);
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    lcd.setCursor (0, 1);
    lcd.print("                ");
    lcd.setCursor (0, 1);
    lcd.print("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}
/* Date         Version       Modification
 * 11/6/2018    v1.00         1.  Added the timer for the project
 *                            2.  Added a function to read the Card.txt file from the SD Card and then organize it
 * 11/9/2018    v1.01         1.  Added pinmode for the RFID Card                           
 * 11/18/2018   v1.02         1.  Added functions for the menu and the navigation
 * 11/22/2018   v1.03         1.  Changed the touch buttons to physical buttons
 *                            2.  Fixed the communication issues with multiple SPI devices 
 *                            3.  Added Real Time Clock and made changes to have cards are saved and compared
 * 11/25/2018   v1.04         1.  Added feature to Add Cards and update Card file
 * 11/25/2018   v1.05         1.  Added feature to Remove Cards and update the Card file 
 * 11/27/2018   v1.06         1.  Added feature to Remove All Cards and update the Card file
 * 12/06/2018   v1.07         1.  Added fingerprint scanner and migrated the fingerprint functions from the component testing section        
 *                            2.  Added fingerprint feature to enroll and delete prints
 *                            3.  Added fingerprint feature to caught if the system has started with no prints to force user to add a print
 */
