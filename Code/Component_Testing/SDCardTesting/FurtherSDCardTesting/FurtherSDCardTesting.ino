#include <SD.h>
#include <SPI.h>

File myFile;

void setup() {
  Serial.begin(9600);
  while(!Serial)
  Serial.println("Starting Initial");
  /*
  if (SD.begin(11)){
    Serial.println("Works");
    myFile = SD.open("Cards.txt", FILE_WRITE);
    Serial.println("Card is good");
    myFile.close();
  }*/
  if (SD.begin(11)){
    Serial.println("Card initalized");
    SD.remove("Cards.txt");
  }
  Serial.println("Done");
}

void loop() {
  // put your main code here, to run repeatedly:

}
