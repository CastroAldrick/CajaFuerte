#include <virtuabotixRTC.h> //This library is needed to run the RTC module.

virtuabotixRTC myRTC(6, 7, 8); //wiring of the pins.
//CLK goes to pin 6
//DAT goes to pin 7
//RST goes to pin 8
void setup() {
 Serial.begin(9600);

// Setting the current date, and time
// seconds, minutes, hours, day of the week, day of the month, month, year

// myRTC.setDS1302Time(05, 25, 17, 7, 15, 9, 2018); //Here you write your actual time/date using the format. 
 
 //After running it one time "comment/remove" this command
 //This setup is only done one time and the module will continue counting automatically.
}

void loop() {
 // This allows for the update of variables for time or accessing the individual elements.
 myRTC.updateTime();

// Start printing date and time
 Serial.print("Current Date / Time: ");
 Serial.print(myRTC.month);
 Serial.print("/");
 Serial.print(myRTC.dayofmonth); 
 Serial.print("/");
 Serial.print(myRTC.year);
 Serial.print(" ");
 Serial.print(myRTC.hours);
 Serial.print(":");
 Serial.print(myRTC.minutes);
 Serial.print(":");
 Serial.println(myRTC.seconds);

 delay(1000);
}
