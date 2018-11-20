/**************************************
  Programmer: Aldrick
  Description: Arduino base Safe project using the Have/Are Authentication
  
                 Wiring
       Component        Arduino 
       
 **************************************/


/**************************************
 Includes      Explanation
 **************************************/
 
#include <TimerOne.h> // For Software interrupts
#include <SD.h> // SD Card Library
#include <SPI.h> // SPI Communication 
#include <MFRC522.h> // RFID Card 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);

unsigned int volatile beat = 10; // 1s
unsigned int volatile buttonDebounceTimer = 5; // 500ms
unsigned int volatile cardScanningTimer = 50; // 5s

//Beating LED
#define BEATING_LED 24

File Files;
char cardBuf[65];
char cards[5][30];

int hLocation = 1;
int menu = 0;
int vLocation = 0;
bool cleared = false;

int successfulRead = 0;
String enteredCard = "";
int resetCardTimer = 0;

// Chip Selects for SPI Slaves
#define CS_RFID     22
#define RST_PIN     23
#define CS_SD_CARD  25
MFRC522 mfrc522(CS_RFID, RST_PIN);

// Navigation buttons
#define UP     46
#define DOWN   48
#define OUT    49
#define IN     47

// Pages Number per Menu
#define PAGES_MAIN_MENU         3
#define PAGES_SAFE_ACCESS       2
#define PAGES_ADMIN_ACCESS      2
#define PAGES_VALID_PRINT       4
#define PAGES_ENROLL_PRINT      7
#define PAGES_DELETE_PRINT      3
#define PAGES_DELETE_ALL        3
#define LOWEST_PAGE             1

struct LCD { 
  String Main[PAGES_MAIN_MENU]            = {"Main Menu",               //  Title
                                      "Safe Access","Admin Access"};    //  Options
                                             
  String Safe_Access[PAGES_SAFE_ACCESS]   = {"Safe Access",             //  Title
                                             "Enter Card"};             //  Options
                                             
  String Admin_Access[PAGES_ADMIN_ACCESS] = {"Admin Access",            //  Title
                                             "Enter Finger"};           //  Options
                                             
  String Valid_Print[PAGES_VALID_PRINT]   = {"Access Granted",                               //  Title
                        "Enroll New Print", "Delete Existing Print", "Erase All Prints"};    // Options
                                             
  String Enroll_Print[PAGES_ENROLL_PRINT] = {"Enroll New Print",                                              //  Title
        "Print ID #", "Enter Finger", "Remove Finger", "Enter Finger", "Finger Enrolled", "No More Room"};    //  Options
      
  String Delete_Print[PAGES_DELETE_PRINT] = {"Delete Print",             //  Title
                                      "Print ID #", "Print Deleted"};    //  Options
  
  String Delete_All[PAGES_DELETE_ALL]     = {"Dlt All Prints",          //  Title
                                    "Are You Sure?", "All Deleted"};    //  Options
}sPage;

void setup() { 
  Serial.begin(9600);  
  pinMode(BEATING_LED, OUTPUT);
  
  pinMode(UP, INPUT);
  pinMode(DOWN , INPUT);
  pinMode(OUT, INPUT);
  pinMode(IN, INPUT);
  
  Timer1.initialize(100000); // 100ms interrupt rate
  Timer1.attachInterrupt( dec );

  lcd.init();
  lcd.backlight();
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  while(!Serial);
  //Serial.println("Here");

  digitalWrite(CS_RFID, LOW);
  digitalWrite(CS_SD_CARD, HIGH);
  
  if (SD.begin(CS_SD_CARD)){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("good");
    fillCardBuffer();
    digitalWrite(CS_SD_CARD, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("no begin");
  }
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
}

void fillCardBuffer() { // Gets all the cards from Sd Card and places them in Card Buffer
  int index = 0;
  int cTotal = 0;
  int cIndex = 0;
  int nextCard = 0;
  
  Files = SD.open("Cards.txt", FILE_READ);

  if (Files){
    Serial.println("Card.txt exist");
    while (Files.available()){
      cardBuf[index] = Files.read();
      index++;
    }
    Files.close();

    /* 
    // Prints card buffer
    for (int i = 0; i < 65; i++){
        Serial.print(i); Serial.print(","); Serial.println(cardBuf[i]);
    }*/

    
    // Organizes the cards 
    for (int i = 0; i < 65; i++){
      
      if ( (i == 11) || (i == 12) ||
           (i == 24) || (i == 25) ||
           (i == 37) || (i == 38) ||
           (i == 50) || (i == 51) ||
           (i == 63) || (i == 64) )
      {
        // Dont Save
      }
      
      if ( (i == 13) || (i == 26) || (i == 39) || (i == 52) )
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
}

int CheckHDirection() { 
  if (digitalRead(UP)) {
    Serial.println("Up has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return 1;
  }

  if (digitalRead(DOWN)) {
    Serial.println("Down has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return -1;
  }
  
  else{return 0;}
}

int CheckVDirection() { 
  if (digitalRead(OUT)) {
    Serial.println("Out has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return -1;
  }

  if (digitalRead(IN)) {
    Serial.println("In has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return 1;
  }
  
  else{return 0;}
}

void LCD_Map(int mode) { 
  
  switch (mode) {
    case 0:
      lcd.setCursor(0,0);
      lcd.print(sPage.Main[0]);
      hLocation = hLocation + CheckHDirection();

      switch (hLocation) {
        case PAGES_MAIN_MENU:
          hLocation = 1;
          break;

        case LOWEST_PAGE - 1:
          hLocation = PAGES_MAIN_MENU - 1;
          break;
      }

      switch (hLocation) {
        case 1:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Main[hLocation]);
          break;

        case 2:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Main[hLocation]);
      }

      vLocation = vLocation + CheckVDirection();

      vLocation = (vLocation == -1)?(0):(vLocation);

      switch (vLocation){
        case 1:
          lcd.clear();
          menu = hLocation;
          vLocation = 0;
          hLocation = 1;
          break;
      }
      break;
      
    case 1:
      lcd.setCursor(0,0);
      lcd.print(sPage.Safe_Access[0]);  
      lcd.setCursor(0,1);
      lcd.print(sPage.Safe_Access[1]);
      
      vLocation = vLocation + CheckVDirection();

      switch (vLocation){
       case -1:
        lcd.clear();
        menu = 0;
        vLocation = 0;
        hLocation = 1;
        break; 
      }

//****// Scan Card
      //Serial.print("card timer = "); Serial.println(cardScanningTimer);
      if (resetCardTimer == 0){
        digitalWrite(CS_RFID, LOW);
        //digitalWrite(CS_SD_CARD, HIGH);
        cardScanningTimer = 50;
        resetCardTimer = 1;
      }
      if (cardScanningTimer > 0){
        successfulRead = getID();
        //Serial.print("READ "); Serial.println(successfulRead);
        if (successfulRead == 1){
          // a Card was scanned
          // check enteredCard
          Serial.println("Card scanned");
          digitalWrite(CS_RFID, HIGH); 
          resetCardTimer = 0;
          lcd.clear();
          cardScanningTimer = 50;     
          menu = 0;
          hLocation = 1;
          vLocation = 0;
          
        }
        else{
          //Serial.println("No card Scanned");
        }
      }
      else{
        // Timer ran out, back to main menu  
        digitalWrite(CS_RFID, HIGH);
        Serial.println("Timed out");
        resetCardTimer = 0;
        lcd.clear();
        cardScanningTimer = 50;     
        menu = 0;
        hLocation = 1;
        vLocation = 0;
      }
      /*delay(5000);
      if (true){
        // Card scans and is good
        // Spins motor for "knob effect"
        // Pulls Solenoid
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Valid Card");
  
        lcd.setCursor(0,1);
        lcd.print("Door is Open");

        // Wait for door to be closed "delay"
        delay(5000);
        lcd.clear();
        menu = 0;
        hLocation = 1;
        vLocation = 0;
      }
      else{
        // Card scans and is bad
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Sorry");
  
        lcd.setCursor(0,1);
        lcd.print("Invalid Card");

        // Wait a little "delay"
        delay(5000);
        lcd.clear();
        menu = 0;
        hLocation = 1;
        vLocation = 0;
      }*/
      break;

    case 2:
      lcd.setCursor(0,0);
      lcd.print(sPage.Admin_Access[0]);
      lcd.setCursor(0,1);
      lcd.print(sPage.Admin_Access[1]);

      vLocation = vLocation + CheckVDirection();

      switch (vLocation){
        case -1:
          menu = 0;
          vLocation = 0;
          hLocation = 1;
          break;  
      }

//****// Finger Scanning    
      if (true){
        lcd.clear();
        menu = 3;
        vLocation = 0;
        hLocation = 1;
      }
      break;

    case 3:
      lcd.setCursor(0,0);
      lcd.print(sPage.Valid_Print[0]);  //  Access Granted
      
      hLocation = hLocation + CheckHDirection();

      switch (hLocation){
        case PAGES_VALID_PRINT:
          hLocation = 1;
          break;        

        case LOWEST_PAGE - 1:
          hLocation = PAGES_VALID_PRINT - 1;
          break;
      }

      switch (hLocation){
        case 1:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]); //  Enroll New Print
          break; 

        case 2:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]);  // Delete Existing Print
          break;

        case 3:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]);  //  Erase All Prints
          break;
      }
      
      vLocation = vLocation + CheckVDirection();
      
      switch (vLocation){
        case -1:
          lcd.clear();
          menu = 0;    //  Main Menu
          vLocation = 0;
          hLocation = 1;
          break;
          
        case 1:
          lcd.clear();
          menu = hLocation + 3;    //  Enroll New Print = 4, Delete Existing = 5, Erase All Prints = 6
          vLocation = 0;
          hLocation = 1;
          break;
      }
      break;

    case 4: //  Enroll New Print Menu
      lcd.setCursor(0,0);
      lcd.print(sPage.Enroll_Print[0]);  //  Enroll New Print
      lcd.setCursor(0,1);
      lcd.print(sPage.Enroll_Print[1]);  //  Print ID # 

      hLocation = hLocation + -CheckHDirection();
      
      switch (hLocation){
        case 0:
          hLocation = 1;
          break;

        case 6:
          hLocation = 5;
          break;
      }
      
      lcd.print("  "); lcd.print(hLocation);

      vLocation = CheckVDirection();

      switch (vLocation){
        case -1:
          lcd.clear();
          menu = 3;
          vLocation = 0;
          hLocation = 1;
          break;  

        case 1:
          vLocation = 0;
          hLocation = 1;
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Enroll_Print[2]);  //  Enter Finger

          // Scan Finger
          delay(2000);
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Enroll_Print[3]);  //  Remove Finger
          
          delay(2000);
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Enroll_Print[4]);  //  Enter Finger

          delay(2000);
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Enroll_Print[5]);  //  Finger Enrolled
          delay(2000);
          lcd.clear();
          menu = 0;
          // Add finger print counter
          break;
      }
      break;

    case 5:
      lcd.setCursor(0,0);
      lcd.print(sPage.Delete_Print[0]);  //  Delete Print
      lcd.setCursor(0,1);
      lcd.print(sPage.Delete_Print[1]);  //  Print ID #

      hLocation = hLocation + -CheckHDirection();
      
      switch (hLocation){
        case 0:
          hLocation = 1;
          break;

        case 6:
          hLocation = 5;
          break;
      }

      lcd.print("  "); lcd.print(hLocation);

      vLocation = CheckVDirection();

      switch (vLocation){
        case -1:
          lcd.clear();
          menu = 3;
          vLocation = 0;
          hLocation = 1;
          break;
        
        case 1:
          //  Delete Print
          delay(2000);
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Delete_Print[2]); lcd.print(" #"); lcd.print(hLocation);
          delay(2000);

          lcd.clear();
          menu = 0;
          break;
      }
      break;

      //  
      case 6:
        lcd.setCursor(0,0);
        lcd.print(sPage.Delete_All[0]);  //  Delete All Prints
        lcd.setCursor(0,1);
        lcd.print(sPage.Delete_All[1]);  //  Are You Sure?
    
        vLocation = CheckVDirection();
  
        switch (vLocation){
          case -1:
            lcd.clear();
            menu = 3;
            vLocation = 0;
            hLocation = 1;
            break;
          
          case 1:
            //  Delete All The Prints
            lcd.setCursor (0, 1);
            lcd.print("                ");
            lcd.setCursor (0, 1);
            lcd.print("Deleting Prints");
            delay(2000);
            lcd.setCursor (0, 1);
            lcd.print("                ");
            lcd.setCursor (0, 1);
            lcd.print(sPage.Delete_All[2]);
            delay(2000);
  
            lcd.clear();
            menu = 0;
            vLocation = 0;
            hLocation = 1;
            break;
        }
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



/* Date         Version       Modification
 * 11/6/2018    v1.00         1.  Added the timer for the project
 *                            2.  Added a function to read the Card.txt file from the SD Card and then organize it
 * 11/9/2018    v1.01         1.  Added pinmode for the RFID Card                           
 * 11/18/2018   v1.02         1.  Added functions for the menu and the navigation
 */
