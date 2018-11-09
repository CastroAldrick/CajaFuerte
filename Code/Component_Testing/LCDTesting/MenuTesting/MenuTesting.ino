/* * * * * * * * * * * * * * * * * *
 *                                 *
 * Program to control the LCD menu *
 *                                 *
 * * * * * * * * * * * * * * * * * */
#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

// Navigation buttons
#define UP     53
#define DOWN   51
#define OUT    49
#define IN     47

// Pages Number per Menu
#define PAGES_MAIN_MENU         3
#define PAGES_SAFE_ACCESS       2
#define PAGES_ADMIN_ACCESS      2
#define PAGES_VALID_PRINT       4

#define TOTAL_PAGES_MAIN_MENU     PAGES_MAIN_MENU - 2
#define TOTAL_PAGES_SAFE_ACCESS   PAGES_SAFE_ACCESS - 1
#define TOTAL_PAGES_ADMIN_ACCESS  PAGES_ADMIN_ACCESS - 1
#define TOTAL_PAGES_VALID_PRINT   PAGES_VALID_PRINT - 1
#define LOWEST_PAGE               1
struct LCD{
  String Main[PAGES_MAIN_MENU] = {"Main Menu", "Safe Access", "Admin Access"};
  String Safe_Access[PAGES_SAFE_ACCESS] = {"Safe Access", "Enter Card"};
  String Admin_Access[PAGES_ADMIN_ACCESS] = {"Admin Access", "Enter Finger"};
  String Valid_Print[PAGES_VALID_PRINT] = {"Admin Access Granted", "Enroll New Print", "Delete Existing Print", "Empty Print Memory"};
}sPage;
int hLocation = 0;
int menu = 0;
int vLocation = 0;

// List of Timers
unsigned int volatile buttonDebounceTimer = 5;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(UP, INPUT);
  pinMode(DOWN , INPUT);
  pinMode(OUT, INPUT);
  pinMode(IN, INPUT);

  Timer1.initialize(100000);
  Timer1.attachInterrupt( Timers );

}

void loop() {
  Schedule();
  //Serial.println(sMenu.Main[0]);
}

void Schedule() {
  //Serial.println(buttonDebounceTimer);
  if (!buttonDebounceTimer) {
    LCD_Map(menu);
  }
}

void Timers() {
  if (buttonDebounceTimer)
    buttonDebounceTimer--;
}
int CheckHDirection() {
  if (digitalRead(UP)) {
    Serial.println("Up has been pressed");
    buttonDebounceTimer = 5;
    return 1;
  }

  if (digitalRead(DOWN)) {
    Serial.println("Down has been pressed");
    buttonDebounceTimer = 5;
    return -1;
  }
  
  else{return 0;}
}
int CheckVDirection(){
  if (digitalRead(OUT)) {
    Serial.println("Out has been pressed");
    buttonDebounceTimer = 5;
    return -1;
  }

  if (digitalRead(IN)) {
    Serial.println("In has been pressed");
    buttonDebounceTimer = 5;
    return 1;
  }
  
  else{return 0;}
}
void LCD_Map(int mode) {
  //Serial.print("Mode is : "); Serial.println(mode);  
  switch (mode) {
    case 0:
      lcd.setCursor(0,0);
      lcd.print(sPage.Main[0]);
      //Serial.print(sMenu.Main[0]); Serial.print(" => "); 
      hLocation = hLocation + CheckHDirection();

      switch (hLocation) {
        case PAGES_MAIN_MENU:
          hLocation = 1;
          break;

        case LOWEST_PAGE - 1:
          hLocation = PAGES_MAIN_MENU - 1;
          break;
      }

      switch (hLocation) {
        case 1:
          lcd.setCursor(0,1);
          lcd.print(sPage.Main[hLocation]);
          //Serial.println(sMenu.Main[hLocation + 1]);
          break;

        case 2:
          lcd.setCursor(0,1);
          lcd.print(sPage.Main[hLocation]);
          //Serial.println(sMenu.Main[hLocation + 1]);
      }

      vLocation = vLocation + CheckVDirection();

      //vLocation = (vLocation == -1)?(0):(vLocation);

      switch (vLocation){
        case 1:
          menu = hLocation + 1;
          vLocation = 0;
          hLocation = 0;
          break;
      }
      break;
      
    case 1:
      lcd.setCursor(0,0);
      lcd.print(sPage.Safe_Access[0]);  
      //Serial.print(sMenu.Safe_Access[0]); Serial.println(" => ");
      break;

    case 2:
      lcd.setCursor(0,0);
      lcd.print(sPage.Admin_Access[0]);
      //Serial.print(sMenu.Admin_Access[0]); Serial.println(" => ");
      break;
  }
}
