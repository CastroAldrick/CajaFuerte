/*
 *                Wiring
 *      Component        Arduino 
 *      
 */



#include <TimerOne.h>
unsigned int volatile beat = 10; //1s
//Beating LED
#define BEATING_LED 24

#include <SD.h> // SD Card Library
#include <SPI.h> // SPI Communication 
#include <MFRC522.h> // RFID Card 

File Files;
char cardBuf[65];
char cards[5][30];

// Chip Selects for SPI Slaves
#define CS_RFID     22
#define CS_SD_CARD  25

void setup() {
  Serial.begin(9600);  
  pinMode(BEATING_LED, OUTPUT);
  pinMode(CS_RFID, OUTPUT);
  pinMode(CS_SD_CARD, OUTPUT);
  
  Timer1.initialize(100000); // 100ms interrupt rate
  Timer1.attachInterrupt( dec );

  while(!Serial);
  Serial.println("Here");

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
  jobs();
}
void jobs(){

  // Indicates that the program is running
  if (!beat){
    digitalWrite(BEATING_LED, digitalRead(BEATING_LED) ^ 1);
    beat = 10;
  }

  // 
  
}
void dec(){
  if (beat)
    beat--;
}
void fillCardBuffer(){ // Gets all the cards from Sd Card and places them in Card Buffer
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

/* Date         Version       Modification
 * 11/6/2018    v1.00         1.  Added the timer for the project
 *                            2.  Added a function to read the Card.txt file from the SD Card and then organize it
 */
