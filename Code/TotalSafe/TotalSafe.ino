/*
   Programmer: Aldrick
   Description: A Safe where to acquire access, you RFID Card must be Valid
   Wiring:
   RFID Scanner -> Arduino Micro
            RST -> 9
        SDA(CS/SS) -> 10
           MOSI -> MOSI
           MISO -> MISO
            SCK -> SCK
            VCC -> 3.3V
            GND -> GND
*/

// Includes
#include <Adafruit_Fingerprint.h>
// MFRC522
#include <SPI.h>
#include <MFRC522.h>
#include "SafeProject.h"

// RTC
#include <virtuabotixRTC.h>
virtuabotixRTC myRTC(6, 7, 8);

// SD Card
#include <SD.h>
#include <SPI.h>
File myFile;
char valid[11] = {'D', 'E', ' ', '2', '6', ' ', '7', 'D', ' ', '6', '3'};
char cardBuf[65];
char cards[5][30];

byte successfulScan;

// LCD Menu
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
char menuScreen = '0';
char newPrint = '0';
char oldPrint = ' ';

int fID = 0;
byte id;
bool rst;

SafeProject safe(true);

void setup() {
  Serial.begin(9600);
  SD.begin(11);
  SPI.begin();

  while (!Serial);
  delay(100);
  Serial.println("Starting...");
}

void loop() {
  if (Serial.available()) {
    menuScreen = Serial.read();
    newPrint = menuScreen;
  }
  //Serial.print("Screen: ");Serial.println(menuScreen);
  switch (menuScreen) {
    case '0':
      if ((newPrint != oldPrint) || rst) {
        Serial.println("Main Menu"); Serial.println("Time\n");
        Serial.println("Waiting for Card");
        int  count = safe.finger.getTemplateCount();
        Serial.println(count);
      }
      oldPrint = newPrint;

      successfulScan = safe.getID();
      if (successfulScan == 1) {
        Serial.print("Card Scanned: "); Serial.println(safe.enteredCard);
        safe.enteredCard = "";
      }
      rst = false;
      break;

    case '1':
      if (newPrint != oldPrint) {
        Serial.println("Main Menu"); Serial.println("Admin Access");
      }
      oldPrint = newPrint;

      fID = safe.getFingerprintID();
      Serial.println(fID);
      if (fID >= 0) {
        Serial.println("Good");
      }
      break;

    case '2':
      if (newPrint != oldPrint) {
        Serial.println("Admin"); Serial.println("Enroll");
        Serial.println("Enter id number: ");
        id = readnumber();
        Serial.print("User: "); Serial.print(id); Serial.println(" Enter finger");
        byte x = getFingerprintEnroll();
      }
      oldPrint = newPrint;
      menuScreen = '0';
      rst = true;
      break;

    case '3':
      // For deleting print
      if (newPrint != oldPrint) {
        Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
        uint8_t id = readnumber();
        if (id == 0)// ID #0 not allowed, try again!
          return;
        Serial.print("Deleting ID #");
        Serial.println(id);
        deleteFingerprint(id);
      }
      menuScreen = '0';
      rst = true;
      break;

    case '4':
      // For emptying fingerprint database
      if (newPrint != oldPrint) {
        Serial.println("Database will now be emptied");
      }
      safe.finger.emptyDatabase();
      menuScreen = '0';
      rst = true;
      break;

    case '5':
      // For adding a card
      
      break;

    case '6':
      // For removing a card
      break;

    case '7':
      // openning the Door
      break;

    case '8':
      // *Getting the time
      if (newPrint != oldPrint) {
        myRTC.updateTime();
        Serial.print("Current Date / Time: ");
        Serial.print(myRTC.month);
        Serial.print("/");
        Serial.print(myRTC.dayofmonth); 
        Serial.print("/");
        Serial.print(myRTC.year);
        Serial.print(" ");
        Serial.print(myRTC.hours);
        Serial.print(":");
        Serial.print(myRTC.minutes);
        Serial.print(":");
        Serial.println(myRTC.seconds);
      }
      menuScreen = '0';
      rst = true;
      break;

    case '9':
      // *Adding info to SD Card
      if (newPrint != oldPrint) {
        myFile = SD.open("Test_9.txt", FILE_WRITE);
        if (myFile){
          myFile.println("Testing Strings\n");
          myFile.println("Testing Numbers");
          myFile.println(123456);
          myFile.println();
          myFile.println("Testing Arrays");
          
          for (byte i = 0; i < 11; i++){
            myFile.print(valid[i]);
          }
          myFile.println();
          myFile.close();
          Serial.println("File Closed");
        }
        else{
          Serial.println("File Doesn't exist");
        }
      }
      menuScreen = '0';
      rst = true;
      break;

    case 'a':
      // *Update Card List on SD Card
      if (newPrint != oldPrint) {
        if (SD.remove("List.txt")){
          Serial.println("List removed");
        }
        myFile = SD.open("List.txt", FILE_WRITE);
        if (myFile){
          Serial.println("File Located");
          for (byte i = 0; i < 65; i++){
            // Buff -> flat array
            // Flat array -> txt file
          }
          myFile.close();
        }
      }
      menuScreen = '0';
      rst = true;
      break;

    case 'b':
      // *Read info from SD Card 
      if (newPrint != oldPrint) {
        myFile = SD.open("List.txt");
        if (myFile){
          Serial.println("File Located");
          byte i = 0;
          while (myFile.available()){
            cardBuf[i] = myFile.read();
            i++;
          }
          myFile.close();

          for (byte i = 0; i < 65; i++){
            
          }
        }
      }
      menuScreen = '0';
      rst = true;
      break;
      
    default:
      if (newPrint != oldPrint) {
        Serial.println("Default");
      }
      oldPrint = newPrint;
      break;
  }
  delay(20);
}




uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintEnroll() {
  safe.finger.begin(57600);
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = safe.finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
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

  p = safe.finger.image2Tz(1);
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
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = safe.finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = safe.finger.getImage();
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

  p = safe.finger.image2Tz(2);
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

  p = safe.finger.createModel();
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
  p = safe.finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
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

  p = safe.finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
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
