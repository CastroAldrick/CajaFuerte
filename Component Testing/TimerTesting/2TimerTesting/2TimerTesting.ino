#include <TimerOne.h>
unsigned int volatile beat = 10; //1s

#include <SD.h>
#include <SPI.h>

File Files;
char cardBuf[65];
char cards[5][30];

void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  
  Timer1.initialize(100000); // 100ms interrupt rate
  Timer1.attachInterrupt( testing );

  while(!Serial);
  
  if (SD.begin(11)){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("aasdf");
    fillCardBuffer();
  }
  else 
    digitalWrite(LED_BUILTIN, HIGH);
}
void loop() {
  beating();
}
void beating(){
  if (!beat){
    digitalWrite(10, digitalRead(10) ^ 1);
    beat = 10;
  }
}
void testing(){
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

    /*
    // Prints saved cards
    for (int i = 0; i < 5; i++){
      for (int j = 0; j < 11; j++){
        Serial.print(i); Serial.print("."); Serial.print(j); Serial.print(" - "); Serial.println(cards[i][j]);
      }*/
      
    }
  }
}

