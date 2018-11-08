/*
 * Programmer: Aldrick
 * Description: Testing of the SD Card
 * Wiring: 
 * 
 * SD Card Reader -> Arduino Micro
 *            GND -> GND
 *            VCC -> 5V
 *           MISO -> MISO
 *           MOSI -> MOSI
 *            SCK -> SCK
 *     SDA(CS/SS) -> 9
 * 
 */

  #include <SD.h>
  #include <SPI.h>

  File myFile;
  char cardBuf[65];
  char cards[5][30];
  char valid[11] = {'D', 'E', ' ', '2', '6', ' ', '7', 'D', ' ', '6', '3'}; 
  int i;
  
void setup() {
  Serial.begin(9600);
  pinMode(22, OUTPUT); digitalWrite(22, LOW);
  while(!Serial)
  Serial.println("Starting Initial");
  if (SD.begin(25))
  {
    Serial.println("Works");
    // *****************************************
    // Reading the Cards in the file and moving 
    // them to a more manageable form factor
    // ***************************************** 
    
    myFile = SD.open("Cards.txt", FILE_READ);
    
    if (myFile){
      Serial.println("Cards.txt exist");
      while (myFile.available()){
        cardBuf[i] = myFile.read();
        i++;
      }
      myFile.close();
      
      int x = 0;
      int cix = 0;
      int next = 0;

      /*
       * Print buffer
       * 
       * for (int i = 0; i < 30; i++){
        Serial.print(i); Serial.print(","); Serial.println(cardBuf[i]);
      }*/
      
      for (int i = 0; i < 65; i++)
      {
        if ( (i != 11) || (i != 12) ||
             (i != 24) || (i != 25) )
        {
          //cards[x][cix] = cardBuf[i];

          if ( (i == 13) || (i == 26) )
          {
            cix = 0;
            next = 1;

            if (next == 1)
              x++;
            cards[x][cix] = cardBuf[i];
            cix++;
            next = 0;
          }
          else
          {
            if (next == 1)
              x++;
            cards[x][cix] = cardBuf[i];
            cix++;
            next = 0;
          }
        }
      }

      // Prints Buffer
      for (int i = 0; i < 65; i++){
        Serial.print(i);Serial.print(". ");Serial.println(cardBuf[i]);
      }
      /*
      // Prints The cards saved
      for (int i = 0; i < 5; i++){
        for (int j = 0; j < 11; j++){
          Serial.print(i); Serial.print("."); Serial.print(j); Serial.println(cards[i][j]);
        }
      }*/
    }
    else
    {
      Serial.println("Error opening Cards.txt");
    }
  }
  else{
    Serial.println("Failed to init");
    return;
  }



  // ***************************
  // Write Info to the SD Card
  // ***************************

  myFile = SD.open("Log.txt", FILE_WRITE);
  if (myFile){
    Serial.println("Lit");
    myFile.println("Testing");
    for (int i = 0; i < 11; i++){
      myFile.print(valid[i]);
    }
    myFile.println();
    myFile.close();
    Serial.println("Done");
  }
  else{
    Serial.println("Not");
  }
}

void loop() {
}
