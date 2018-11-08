
  //saving the card
  /*
  for (int i = 0; i <= (n+1); i++) {
    if (i < (n+1)){
      EEPROM.write(i, char_array[i]);
    }
    else{
      EEPROM.write((n+1), 41);
    }
  }*/
  //Looks for ending of card 
  /*
  for (int i = 0; i <= (n+1); i++){
    Serial.print(char(EEPROM.read(i)));
    if (char(EEPROM.read(i)) == ')'){
      Serial.print(" end is at ");
      Serial.println(i);
    }
  }*/

  //Looks for empty space for next card
  /*
  for (int i = 0; i <= 1024; i++){
    //Serial.println(EEPROM.read(i));
    if ((EEPROM.read(i) == 41) && (EEPROM.read(i+1) == 0)){
      Serial.println(i);
      int x = i+1;
      for (x; (x-(i+1)) <= (n+1); x++) {
        EEPROM.write(x, char_array[(x-(i+1))]);
        if (x-(i+1) == (n+1)){
          EEPROM.write(x,41);
        }
      }
    }
  }
  */
  
  //Cycle through EEPROM
  /*
  for (int i = 0; i <= 1024; i++){
    if (EEPROM.read(i) != 0){
      Serial.print(i);
      Serial.print("\t");
      Serial.println(char(EEPROM.read(i)));
    }
  }
  */

  //Manually editing eeprom
    /*
    EEPROM.write(26,41);
    EEPROM.write(1024,0);
    */

  //Clear EEPROM
  /*
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  */
