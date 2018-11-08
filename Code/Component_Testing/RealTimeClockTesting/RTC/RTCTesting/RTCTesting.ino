#include <DS3231.h>
DS3231 rtc(SDA, SCL);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  rtc.begin();

  //rtc.setDOW(WEDNESDAY);
  //rtc.setTime(7, 22, 0);
  //rtc.setDate(19, 9, 2018);
}

void loop() {
  Serial.print(rtc.getDOWStr());Serial.print(" ");
  Serial.print(rtc.getDateStr());Serial.print(" ");
  Serial.println(rtc.getTimeStr());
  delay(1000);
}
