
int CheckHDirection() { 
  if (!digitalRead(LEFT)) {
    Serial.println("LEFT has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return -1;
  }

  if (!digitalRead(RIGHT)) {
    Serial.println("RIGHT has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return 1;
  }
  else{return 0;}
}

int CheckVDirection() { 
  if (!digitalRead(BACK)) {
    Serial.println("BACK has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return -1;
  }

  if (!digitalRead(ENTER)) {
    Serial.println("ENTER has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return 1;
  }
  else{return 0;}
}

void LCD_Map(int mode) {  
  // Pages Number per Menu
  #define PAGES_MAIN_MENU         3
  #define PAGES_SAFE_ACCESS       2
  #define PAGES_ADMIN_ACCESS      2
  #define PAGES_VALID_PRINT       7
  #define PAGES_ENROLL_PRINT      7
  #define PAGES_DELETE_PRINT      3
  #define PAGES_DELETE_ALL        3
  #define PAGES_ADD_CARD          4
  #define PAGES_REMOVE_CARD       5
  #define PAGES_REMOVE_ALL_CARDS  3
  #define LOWEST_PAGE             1
  
  struct LCD { 
    String Main[PAGES_MAIN_MENU]            = {"Main Menu",               //  Title
                                        "Safe Access","Admin Access"};    //  Options
                                               
    String Safe_Access[PAGES_SAFE_ACCESS]   = {"Safe Access",             //  Title
                                               "Enter Card"};             //  Options
                                               
    String Admin_Access[PAGES_ADMIN_ACCESS] = {"Admin Access",            //  Title
                                               "Enter Finger"};           //  Options
                                               
    String Valid_Print[PAGES_VALID_PRINT]   = {"Access Granted",                               //  Title
                          "Enroll New Print", "Delete Existing Print", "Erase All Prints",     // Options
                          "Add Card", "Remove Card", "Remove All Cards"};    
                                               
    String Enroll_Print[PAGES_ENROLL_PRINT] = {"Enroll New Print",                                              //  Title
          "Print ID #", "Enter Finger", "Remove Finger", "Enter Finger", "Finger Enrolled", "No More Room"};    //  Options
        
    String Delete_Print[PAGES_DELETE_PRINT] = {"Delete Print",             //  Title
                                        "Print ID #", "Print Deleted"};    //  Options
    
    String Delete_All[PAGES_DELETE_ALL]     = {"Dlt All Prints",          //  Title
                                      "Are You Sure?", "All Deleted"};    //  Options
                                      
    String Add_Card[PAGES_ADD_CARD]         = {"Add Card.Space ",                               //  Title
                                    "Scan Card", "Card is #: ", "No More Space"};     //  Options
                                      
    String Remove_Card[PAGES_REMOVE_CARD]   = {"Remove Card",             //  Title
                               "Scan Card", "Card # ", "Card Removed", "No Cards Saved"} ;   //  Options
                                      
    String Remove_All_Cards[PAGES_REMOVE_ALL_CARDS]   = {"Remove All Cards",                       //  Title
                                                   "Are You Sure?", "All Removed"};     //  Options
  }sPage; 

  bool emptySlot[5] = {false, false, false, false, false};
  bool occupiedSlot[5] = {false, false, false, false, false};
  bool match = false;
  int attempt = 0; 
  int emptySpace = 0;
  int occupiedSpace = 0;
  int fingerCount = 0;

  //digitalWrite(CS_RFID, HIGH);
  //digitalWrite(CS_SD_CARD, HIGH);
  
  switch (mode) {
    case 0:
      finger.getTemplateCount();
      fingerCount = finger.templateCount;

      if (fingerCount == 0){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("No Fingerprints");
        lcd.setCursor(0,1);
        lcd.print("Are Enrolled");
        delay(2000);
        menu = 4;
      }
      digitalWrite(CS_RFID, HIGH);
      digitalWrite(CS_SD_CARD, HIGH);
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
        digitalWrite(CS_SD_CARD, HIGH);
        cardScanningTimer = 50;
        resetCardTimer = 1;
      }
      if (cardScanningTimer > 0){
        successfulRead = getID();
        //Serial.print("READ "); Serial.println(successfulRead);
        if (successfulRead == 1){
          // a Card was scanned
          Serial.println("Card scanned");
          Serial.println(enteredCard);
          //digitalWrite(CS_RFID, HIGH);


          // Check if card is Good
          for (int num = 0; num <= 4; num++){
            if (enteredCard == sCards[num]){
              Serial.print(enteredCard);
              Serial.print("Matches Card ");
              Serial.println(num);
              match = true;
              break;
            }
            else if(num == 4)
              Serial.println("No Match was found");
          }

          if (match){
            //  pull solenoid
            //  Check hall effect sensor
          }
            
            
          
          
          // Save card scanned in LOG.TXT file along with time and Status
          //digitalWrite(CS_SD_CARD, LOW);
          Files = SD.open("LOG.TXT", FILE_WRITE);
          if (Files)
          {
            Serial.println("LOG file open");
            Files.println();
            Files.print(enteredCard);
            Files.print("\t");
            Files.print(rtc.getDateStr());
            Files.print("\t");
            Files.print(rtc.getTimeStr());

            if (match){
              Files.print("\t");
              Files.println("Valid");
            }
            Files.close();
          }
          else
          {
            Serial.println("Failed");
          }

          enteredCard = "";
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
      if (resetFingerScanningTimer == 0){
        fingerScanTimer = 50;
        resetFingerScanningTimer = 1;
      }
      
      if (fingerScanTimer > 0){
        fingerprintID = getFingerprintID();
        if (fingerprintID != -1){
          Serial.println("Good");
          lcd.clear();
          fingerprintID = -1;
          resetFingerScanningTimer = 0;
          menu = 3;
          hLocation = 1;
          vLocation = 0;
        }
      }
      else{
        Serial.println("Took too long");
        lcd.clear();
        resetFingerScanningTimer = 0;
        fingerprintID = -1;
        menu = 0;
        hLocation = 1;
        vLocation = 0; 
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
          lcd.print(sPage.Valid_Print[hLocation]);  //  Add Card
          break;
          
        case 4:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]); //  Remove Card
          break;
          
        case 5:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]); //  Remove All Cards
          break;
          
        case 6:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]); //  Enroll New Print
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
          menu = hLocation + 3;    
          //  Enroll New Print = 4, 
          //  Delete Existing = 5, 
          //  Erase All Prints = 6,
          //  Add Card = 7,
          //  Remove Card = 8,
          //  Remove All Cards = 9
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

      hLocation = hLocation + CheckHDirection();
      
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
          if (fingerCount == 0){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Please Enroll a");
            lcd.setCursor(0,1);
            lcd.print("Finger to Start");
            delay(3000);
            lcd.clear();
          }
          else{
            lcd.clear();
            menu = 3;
            vLocation = 0;
            hLocation = 1;
          }
          break;  

        case 1:
          vLocation = 0;
          //hLocation = 1;

          getFingerprintEnroll(hLocation);

          /*
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
          */
          
          lcd.clear();
          menu = 0;
          hLocation = 1;
          // Add finger print counter
          break;
      }
      break;

    case 5:
      lcd.setCursor(0,0);
      lcd.print(sPage.Delete_Print[0]);  //  Delete Print
      lcd.setCursor(0,1);
      lcd.print(sPage.Delete_Print[1]);  //  Print ID #

      hLocation = hLocation + CheckHDirection();
      
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
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Delete_Print[2]); lcd.print(" #"); lcd.print(hLocation);
          delay(1000);
          deleteFingerprint(hLocation);

          lcd.clear();
          menu = 0;
          vLocation = 0;
          hLocation = 1;
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
        break;

      case 7: //  Add Card
        lcd.setCursor(0,0);
        lcd.print(sPage.Add_Card[0]);  //  Add Card

        for (int card = 0; card <= 4; card++){
          if (sCards[card] == "EMPTY EMPTY"){
            Serial.println(card);
            emptySlot[card] = true;
            emptySpace++;

            if (emptySlot[card]){
              Serial.print("Slot ");
              Serial.print(card);
              Serial.println(" empty");
            }
          }
        }

        lcd.print(emptySpace);
        //delay(5000);
        hLocation = hLocation + CheckHDirection();

        switch (hLocation){
          case LOWEST_PAGE - 1:
            hLocation = PAGES_ADD_CARD - 1;
            break;
          
          case PAGES_ADD_CARD:
            hLocation = LOWEST_PAGE;
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
            lcd.print(sPage.Add_Card[hLocation]);
            break;

          case 2:
            if (cleared == false){
              lcd.setCursor (0, 1);
              lcd.print("                ");
              cleared = true; 
            }
            lcd.setCursor(0,1);
            lcd.print(sPage.Add_Card[hLocation]);
            break;

          case 3:
            if (cleared == false){
              lcd.setCursor (0, 1);
              lcd.print("                ");
              cleared = true; 
            }
            lcd.setCursor(0,1);
            lcd.print(sPage.Add_Card[hLocation]);
            break;
        }

        vLocation = vLocation + CheckVDirection();
      
        switch (vLocation){
          case -1:
            lcd.clear();
            menu = 2;    //  Admin Access  
            vLocation = 0;
            hLocation = 1;
            break;
            
          case 1:
            lcd.clear();
            menu = 10; //  Scan Card for adding
            vLocation = 0;
            hLocation = 1;
            break;
        }
        /*for (int inx = 0; inx <= 4; inx++){
                  Serial.println(emptySlot[inx]);}*/
        break;

      case 8:
        lcd.setCursor(0,0);
        lcd.print(sPage.Remove_Card[0]);

        for (int card = 0; card <= 4; card++){
//          if (sCards[card] != "EMPTY EMPTY"){
//            
//            Serial.print(card);
//            Serial.println("Card is empty");
              occupiedSlot[card] = true;
              occupiedSpace++;
        }
        

//        Serial.println(occupiedSpace);
        if (occupiedSpace == 0){
          Serial.println("No cards exist");
          lcd.clear();
          menu = 2;    //  Admin Access  
          vLocation = 0;
          hLocation = 1;
        }
        
         vLocation = vLocation + CheckVDirection();
      
        switch (vLocation){
          case -1:
            lcd.clear();
            menu = 2;    //  Admin Access  
            vLocation = 0;
            hLocation = 1;
            break;
            
          case 1:
            lcd.clear();
//            menu = 11; //  Scan Card for removing
//            vLocation = 0;
//            hLocation = 1;
              lcd.setCursor(0,0);
              lcd.print(sPage.Remove_Card[2]);

              if (resetCardTimer == 0){
                digitalWrite(CS_RFID, LOW);
                digitalWrite(CS_SD_CARD, HIGH);
                cardScanningTimer = 50;
                resetCardTimer = 1;
              }

              if (cardScanningTimer > 0){
                Serial.println("Scanning");
                successfulRead = getID();
                if (successfulRead == 1){
                  digitalWrite(CS_RFID, HIGH);
                  Serial.println(enteredCard);

                  for (int card = 0; card <= 4; card++){
                    if (enteredCard == sCards[card]){
                      Serial.print(enteredCard);
                      Serial.print(" Matches ");
                      Serial.println(card);
                      Serial.println("Removing");
                      sCards[card] = "EMPTY EMPTY";
                      updateCard();
                      Serial.println("Removed");
                      break;
                      
                    }
                    else if(card == 4){
                      Serial.println("Doesnt match records");
                    }
                  }
                  enteredCard = "";
                  resetCardTimer = 0;
                  cardScanningTimer = 50;
                  lcd.clear();
                  menu = 0;    //  Main Menu 
                  vLocation = 0;
                  hLocation = 1;  
                }
              }
            Serial.println(sCards[0]);
            Serial.println(sCards[1]);
            Serial.println(sCards[2]);
            Serial.println(sCards[3]);
            Serial.println(sCards[4]);
            break;
        }
        break;

      case 9:  //  Remove All Cards
        lcd.setCursor(0,0);
        lcd.print(sPage.Remove_All_Cards[0]); // Remove All Cards

        lcd.setCursor(0,1);
        lcd.print(sPage.Remove_All_Cards[1]); // Are You Sure?

        vLocation = CheckVDirection();

        switch (vLocation){
          case -1:  // Back was pressed
            lcd.clear();
            menu = 3;
            vLocation = 0;
            hLocation = 1;
            break;

          case 1:  // Enter was pressed
            lcd.setCursor(0,1);
            lcd.print("                ");
            lcd.setCursor(0,1);
            lcd.print("Deleting all Cards");
            
            for (int i = 0; i <= 4; i++){
              sCards[i] = "EMPTY EMPTY";
            }
            rmAllCards();
            updateCard();

            lcd.clear();
            menu = 0;
            vLocation = 0;
            hLocation = 1; 
            break;
        }
        break;

      case 10:
        lcd.setCursor(0,0);
        lcd.print(sPage.Add_Card[1]);  //  Scan Card for adding

        for (int card = 0; card <= 4; card++){
          if (sCards[card] == "EMPTY EMPTY"){
            //Serial.println(card);
            emptySlot[card] = true;

            /*if (emptySlot[card]){
              Serial.print("Slot ");
              Serial.print(card);
              Serial.println(" empty");
            }*/
          }
        }
        Serial.print("reset ");
        Serial.println(resetCardTimer);
        if (resetCardTimer == 0){
          digitalWrite(CS_RFID, LOW);
          digitalWrite(CS_SD_CARD, HIGH);
          Serial.println("here");
          cardScanningTimer = 50;
          resetCardTimer = 1;
        }

        Serial.print("timer ");
        Serial.println(cardScanningTimer);
        if (cardScanningTimer > 0){
          successfulRead = getID();
          Serial.print("success ");
          Serial.println(successfulRead);
          if (successfulRead == 1){
            // a Card was scanned
            Serial.println("Card scanned");
            Serial.println(enteredCard);
            digitalWrite(CS_RFID, HIGH);

            // Check if card already exist
            for (int num = 0; num <= 4; num++){
              if (enteredCard == sCards[num]){
                Serial.print(enteredCard);
                Serial.print(" Matches Card ");
                Serial.println(num);
                Serial.println("Cannot Add");
                for (int i = 0; i <= 4; i++){
                  Serial.println(sCards[i]);
                }

                
                //match = true;
                enteredCard = "";
                resetCardTimer = 0;
                cardScanningTimer = 50;
                lcd.clear();
                menu = 0;    //  Main Menu 
                vLocation = 0;
                hLocation = 1;
                break;
              }
              else if(num == 4){
                Serial.println("No Match was found");

                for (int inx = 0; inx <= 4; inx++){
                  //Serial.println(emptySlot[inx]);
                  
                  if (emptySlot[inx]){
                    sCards[inx] = enteredCard;
                    Serial.println("Card added");

                    //  upload changes to sd card
                    Serial.println(sCards[0]);
                    Serial.println(sCards[1]);
                    Serial.println(sCards[2]);
                    Serial.println(sCards[3]);
                    Serial.println(sCards[4]);

                    enteredCard = "";
                    resetCardTimer = 0;
                    cardScanningTimer = 50;
                    lcd.clear();
                    menu = 0;    //  Main Menu 
                    vLocation = 0;
                    hLocation = 1;
                    updateCard();
                    break;
                  }
                  else
                    Serial.println("blah");
                }
              }
            }
          }
        }
        break;
  }
}


