/*
  Serial.println("hello");
  /*getID();
  if (enteredCard != "")
  {
    if (enteredCard == validCard)
      digitalWrite(12, HIGH);
  }
  delay(1000);
  digitalWrite(12, LOW);
  enteredCard = "";
  
  
  //EEPROM Challenge
  //EEPROM.write(0,41);
  int n = enteredCard.length();
  
  if (check == 0){
    //addCard(n);
  }
  
  char char_array[n + 1];
  strcpy(char_array, enteredCard.c_str());
  
  Serial.println();

  //Looking for specific card
  //parameter will be the scanned card
  /*
  for (int i = 0; i <= EEPROM.length(); i++){
    if (EEPROM.read(i) == 40){
      //x is the card index
      for (int x = i; x <= (n+i); x++){
        if (EEPROM.read(i+1) == card_Array[i]){
          
        }  
      }
    }
  }  

  //clearEE();
  //cycleEE();
  //Attempt 2
  String allCards[5][11];
  
  for (int i = 0; i <= 4; i++){
    for (int j = 0; i <= 10; j++){
      allCards[i][j] = char_array[j];
    }
  }

  for (int i = 0; i <= 10; i++){
    Serial.print(allCards[0][i]);
  }
  */
