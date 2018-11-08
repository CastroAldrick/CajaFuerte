/*
 * Programmer: Aldrick
 * Description: Testing of EEPROM
 * Wiring:
 */

#include <EEPROM.h>
int found = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  char charArray[] = {'(', ' ', ' ', ')', '(', 'A', 'B', ')'};
  
  for (int i = 0; i <= 7; i++){
    Serial.print(charArray[i]);
    Serial.print(",");
  }
  Serial.println("\nSetup Done...");
  
  char enteredCardArray[] = {'A', 'B'};
  rmCard(enteredCardArray , charArray);
  for (int i = 0; i <= 7; i++){
    Serial.print(charArray[i]);
    Serial.print(",");
  }
  Serial.println();

  addCard(enteredCardArray, charArray);
  for (int i = 0; i <= 7; i++){
    Serial.print(charArray[i]);
    Serial.print(",");
  }
  Serial.println();
  cleanEEPROM();
  for (int i = 0; i <= 7; i++){
    Serial.print(char(EEPROM.read(i)));
    Serial.print(",");
  }
  Serial.println();
  
  updateEEPROM(charArray);
  for (int i = 0; i <= 7; i++){
    Serial.print(char(EEPROM.read(i)));
    Serial.print(",");
  }
}


void loop() {
}

void rmCard(char *card, char *allCards){
  bool removed = false;
  char cards;
  byte *cardsAddress;

  char rmCard;
  byte *rmCardAddress;
  
  for (int i = 0; i <= 8; i++){ 
    cardsAddress = (allCards+i);
    cards = *cardsAddress;

    rmCardAddress = (card+i);
    rmCard = *rmCardAddress;
    
    if (removed){
      return;
    }
    else{
      if (cards == '('){
        if (found == 0)
         found = i;
        Serial.println("Found a Card...");
        if ( (*(allCards+i+1)) == (*(card+(i-found))) &&
             (*(allCards+i+2)) == (*(card+(i-found+1))) ){
          Serial.print(*(allCards+i+1)); Serial.print("\t"); Serial.println(*(card+(i-found)));
          Serial.print(*(allCards+i+2)); Serial.print("\t"); Serial.println(*(card+(i-found+1)));
          Serial.println("Match");
          //Clear Card
          *(allCards+i+1) = ' ';
          *(allCards+i+2) = ' ';
          removed = true;
        }
        else{
          Serial.println("No Match");
          found = 0;
        } 
      } 
    }
  }
}

void addCard(char *card, char *allCards){
  int added = 0;
  char cards;
  byte *cardsAddress;

  char addCard;
  byte *addCardAddress;

  for (int i = 0; i <= 8; i++){
    if (added != 0){
      return;
    }
    else{
      cardsAddress = allCards+i;
      cards = *(cardsAddress);
    
      addCardAddress = card+i;
      addCard = *(addCardAddress);
      if (cards == '('){
         Serial.print("( is in: "); Serial.println(i);
         if (*(allCards+i+1) == ' '){
             Serial.println("Found an empty spot");
             for (int j = 0; j < 2; j++){
               *(allCards+i+j+1) = *(card+j);
             }
             added = 1;
         }
         else
           Serial.println("Not an empty spot");
       }
    } 
  }
}

void cleanEEPROM(){
  Serial.println("Cleaning");
  for (int i = 0; i <= 7; i++){
    if ((i == 0) || (i == 4)){
      EEPROM.write(i,40); //40 == ( in ASCII 
    }
    else if ((i == 3) || (i == 7)){
      EEPROM.write(i,41); //41 == ) in ASCII
    }
    else{
      EEPROM.write(i,127); //127 == SPACE in ASCII
    }
  }
}

void updateEEPROM(char *allCards){
  Serial.println("Updating");
  byte *cardsAddress;
  char cards;
  
  for (int i = 0; i <= 7; i++){
    cardsAddress = allCards+i;
    cards = *cardsAddress;
    EEPROM.write(i,cards);
  }
}

