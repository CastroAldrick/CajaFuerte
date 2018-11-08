#include <TimerOne.h>

unsigned int volatile One = 1;
unsigned int volatile Twenty = 20;
unsigned int volatile Thirty = 30;

void setup() {
  Serial.begin(9600);
  Timer1.initialize(1000000); // 1 Sec interrupt rate
  Timer1.attachInterrupt( printing );
}

void loop() {
  if (!One){
    Serial.println("1 Second Passed");
    One = 1;
  }
  if (!Twenty){
    Serial.println("20 Second Passed");
    Twenty = 20;
  }
  if (!Thirty){
    Serial.println("30 Seconds Passed");
    Thirty = 30;
  }
}

void printing(){
  if (One)
    One--;
  if (Twenty)
    Twenty--;
  if (Thirty)
    Thirty--;
}

