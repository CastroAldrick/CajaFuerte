#include <DS3231.h>
#include <SD.h>
#include <SPI.h>

File myFile;
char valid[11] = {'D', 'E', ' ', '2', '6', ' ', '7', 'D', ' ', '6', '3'};
DS3231 rtc(SDA, SCL);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting Initial");
  rtc.begin();

  if (SD.begin(9))
  {
    Serial.println("Works");
    myFile = SD.open("Testing.txt", FILE_WRITE);

    if (myFile)
    {
      Serial.println("Testing.txt exist");
      if (myFile)
      {
        myFile.print(rtc.getDOWStr()); myFile.print(" ");
        myFile.print(rtc.getDateStr()); myFile.print(" ");
        myFile.print(rtc.getTimeStr()); myFile.print(": ");

        for (int i = 0; i < 11; i++)
        {
          myFile.print(valid[i]);
        }
        myFile.println();
        myFile.close();
        Serial.println("Done");
      }
      else
      {
        Serial.println("Wasnt available");
      }
    }
  }

  /*rtc.setDOW(Day of the week);
  rtc.setTime(hour, minute, second);
  rtc.setDate(day, month, year);*/
}

void loop() {

}
