/* * * * * * * * * * * * * * * * * *
 *                                 *
   Program to control the LCD menu
 *                                 *
 * * * * * * * * * * * * * * * * * */
#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

typedef int BUTTON;
typedef const int PAGES;

BUTTON up = 53;
BUTTON down = 51;
BUTTON out = 49;
BUTTON in = 47;

bool upState = false;
bool downState = false;
bool outState = false;
bool inState = false;

// List of Timers
unsigned int volatile buttonDebounceTimer = 5;

// Amount of Pages for each Menu
PAGES LCD_LINES = 2;
PAGES LCD_TOP_LINE = 6;
PAGES LCD_MENUS = 10;

int pointer = 0;
int menu = 0;

PAGES MAIN_PAGES = 2;
PAGES ACCESS_ATTEMPT_PAGES = 1;
PAGES ADMIN_ATTEMPT_PAGES = 1;
PAGES ADMIN_ACCESS_PAGES = 3;

// Pages
String LCD_TITLES [LCD_MENUS] = {
  "Main Menu",
  "Safe Access",
  "Admin Access",
  "Enter Card",
  "Enter Finger",
  "Valid",
  "Invalid",
  "Enroll New Print",
  "Delete Print",
  "Empty Saved Prints"
};
/*
  String LCD_Main          [LCD_LINES][MAIN_PAGES]            = { {"Main",        "Access Safe"}, {"Main", "Admin Access"} };
  String LCD_Access_Attempt[LCD_LINES][ACCESS_ATTEMPT_PAGES]  = { {"Access Safe"} ,        {"Enter Your Card"}             };
  String LCD_Admin_Attempt [LCD_LINES][ADMIN_ATTEMPT_PAGES]   = { {"Admin Access"} ,       {"Enter Your Finger"}           };
  String LCD_Admin_Access  [LCD_LINES][ADMIN_ACCESS_PAGES]    = { {"Admin Mode"}{}{} }
*/

void setup() {
  Serial.begin(9600);

  pinMode(up, INPUT);
  pinMode(down , INPUT);
  pinMode(out, INPUT);
  pinMode(in, INPUT);

  Timer1.initialize(100000);
  Timer1.attachInterrupt( Timers );
}

int place = 5;

void loop() {
  Schedule();
}

void Schedule() {
  //Serial.println(buttonDebounceTimer);
  if (!buttonDebounceTimer) {
    place=place + CheckHDirection();
    Serial.println(place);
    //Serial.println(CheckVDirection());
    //LCD_MAP(pointer);
  }
}

void Timers() {
  if (buttonDebounceTimer)
    buttonDebounceTimer--;
}
int CheckHDirection() {
  if (digitalRead(53)) {
    Serial.println("Up has been pressed");
    buttonDebounceTimer = 5;
    return 1;
  }

  if (digitalRead(51)) {
    Serial.println("Down has been pressed");
    buttonDebounceTimer = 5;
    return -1;
  }
  
  else{return 0;}
}
int CheckVDirection(){
  if (digitalRead(49)) {
    Serial.println("Out has been pressed");
    buttonDebounceTimer = 5;
    return -1;
  }

  if (digitalRead(47)) {
    Serial.println("In has been pressed");
    buttonDebounceTimer = 5;
    return 1;
  }
}

/*
void LCD_Map(int movement) {
  switch (movement) {

    case '0':
      Serial.print(LCD_TILES[0]); 
      Serial.println(LCD_TITLES[1]);
      break;
  }
}
*/
