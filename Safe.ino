// Project Name: Arduino RFID Safe

#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
using namespace std;

#define SS_PIN  10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
String enteredCard = "C9 07 6A F3";
String validCard = "B5 07 F2 C2";
String eCard = "";
int check = 0;
int x = 0;

void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Ready");
}

void loop() {
  Serial.println("In Loop");
  delay(1000);

  //clearEE();
  //cycleEE();
  
  int n = enteredCard.length();
  char char_array[n + 1];
  strcpy(char_array, enteredCard.c_str());
  
  char allCards[5][11];

  for (int i = 0; i <= 10; i++){
    allCards[0][i] = char_array[i];
    //Serial.print(allCards[0][i]);
    EEPROM.write(i, allCards[0][i]);
    Serial.print(char(EEPROM.read(i)));
  }
  
  
  for (int i = 0; i <= 10; i++){
    //Serial.println(allCards[0][i]);
    //EEPROM.write(i, char_array[i]);
  }
  
  
  /*
  for (int i = 0; i <= 4; i++){
    for (int j = 0; i <= 10; j++){
      allCards[i][j] = char_array[j];
    }
  }
  

  for (int i = 0; i <= 10; i++){
    Serial.print(allCards[0][i]);
  }*/
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

uint8_t getID(){
  if (! mfrc522.PICC_IsNewCardPresent())
  {
    return 0;
  }
  if (! mfrc522.PICC_ReadCardSerial())
  {
    return 0;
  }
  Serial.println(mfrc522.uid.size);
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (i == 0){
      enteredCard.concat("40");
    }
    enteredCard.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    enteredCard.concat(String(mfrc522.uid.uidByte[i], HEX ));
  }
  enteredCard.toUpperCase();
  Serial.println(enteredCard/*.substring(1)*/);
  mfrc522.PICC_HaltA();
  return 1;
}

void addCard(int n){
  char char_array[n + 1];
  strcpy(char_array, enteredCard.c_str());
  
  for (int i = 0; i <= 1024; i++){
    if ((EEPROM.read(i) == 41) && (EEPROM.read(i+1) == 0)){
      //Write 40 '(' first
      EEPROM.write((i+1), 40);
      
      //EEPROM ADDRESS; ARRAY VALUE <= ENDING ARRAY VALUE
      for (int x = (i+2); (x-(i+2)) <= n; x++) {
        //End of the card id
        if (x-(i+2) == n){
          EEPROM.write(x, 41);
          check++;
        }
        else{
          EEPROM.write(x, char_array[ (x-(i+2)) ]);
        }
      }
    }
    else if (check != 0){
      Serial.println("Card added");
      return;
    }
  }
}

void cycleEE(){
  for (int i = 0; i <= 1024; i++){
    if (EEPROM.read(i) != 0){
      Serial.print(i);
      Serial.print("\t");
      Serial.println(char(EEPROM.read(i)));
    }
    else{
      Serial.print(i);
      Serial.print("\t");
      Serial.println("Nothing");
    }
  } 
}

void clearEE(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}
/*
   08/22/2018 Adding RFID Functionality
   08/26/2018 Adding EEPROM Functions
*/
