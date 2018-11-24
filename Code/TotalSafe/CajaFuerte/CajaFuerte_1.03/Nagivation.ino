
int CheckHDirection() { 
  if (!digitalRead(LEFT)) {
    Serial.println("LEFT has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return 1;
  }

  if (!digitalRead(RIGHT)) {
    Serial.println("RIGHT has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return -1;
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
  bool match = false;
  int attempt = 0;
  
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
          digitalWrite(CS_RFID, HIGH);


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
          digitalWrite(CS_SD_CARD, LOW);
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

