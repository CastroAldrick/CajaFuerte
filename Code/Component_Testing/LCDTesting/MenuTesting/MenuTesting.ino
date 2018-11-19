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
#define PAGES_ENROLL_PRINT      7
#define PAGES_DELETE_PRINT      3
#define PAGES_DELETE_ALL        3
#define LOWEST_PAGE             1

struct LCD{
  String Main[PAGES_MAIN_MENU]            = {"Main Menu",               //  Title
                                      "Safe Access","Admin Access"};    //  Options
                                             
  String Safe_Access[PAGES_SAFE_ACCESS]   = {"Safe Access",             //  Title
                                             "Enter Card"};             //  Options
                                             
  String Admin_Access[PAGES_ADMIN_ACCESS] = {"Admin Access",            //  Title
                                             "Enter Finger"};           //  Options
                                             
  String Valid_Print[PAGES_VALID_PRINT]   = {"Access Granted",                               //  Title
                        "Enroll New Print", "Delete Existing Print", "Erase All Prints"};    // Options
                                             
  String Enroll_Print[PAGES_ENROLL_PRINT] = {"Enroll New Print",                                              //  Title
        "Print ID #", "Enter Finger", "Remove Finger", "Enter Finger", "Finger Enrolled", "No More Room"};    //  Options
      
  String Delete_Print[PAGES_DELETE_PRINT] = {"Delete Print",             //  Title
                                      "Print ID #", "Print Deleted"};    //  Options
  
  String Delete_All[PAGES_DELETE_ALL]     = {"Dlt All Prints",          //  Title
                                    "Are You Sure?", "All Deleted"};    //  Options
}sPage;

int hLocation = 1;
int menu = 0;
int vLocation = 0;
bool cleared = false;

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
  Serial.println(digitalRead(UP));
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
    cleared = false;
    return 1;
  }

  if (digitalRead(DOWN)) {
    Serial.println("Down has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return -1;
  }
  
  else{return 0;}
}
int CheckVDirection(){
  if (digitalRead(OUT)) {
    Serial.println("Out has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return -1;
  }

  if (digitalRead(IN)) {
    Serial.println("In has been pressed");
    buttonDebounceTimer = 5;
    cleared = false;
    return 1;
  }
  
  else{return 0;}
}
void LCD_Map(int mode) { 
  
  switch (mode) {
    case 0:
      lcd.setCursor(0,0);
      lcd.print(sPage.Main[0]);
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
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Main[hLocation]);
          break;

        case 2:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Main[hLocation]);
      }

      vLocation = vLocation + CheckVDirection();

      vLocation = (vLocation == -1)?(0):(vLocation);

      switch (vLocation){
        case 1:
          lcd.clear();
          menu = hLocation;
          vLocation = 0;
          hLocation = 1;
          break;
      }
      break;
      
    case 1:
      lcd.setCursor(0,0);
      lcd.print(sPage.Safe_Access[0]);  
      lcd.setCursor(0,1);
      lcd.print(sPage.Safe_Access[1]);
      
      vLocation = vLocation + CheckVDirection();

      switch (vLocation){
       case -1:
        lcd.clear();
        menu = 0;
        vLocation = 0;
        hLocation = 1;
        break; 
      }
      
//****// Scan Card
      delay(5000);
      if (true){
        // Card scans and is good
        // Spins motor for "knob effect"
        // Pulls Solenoid
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Valid Card");
  
        lcd.setCursor(0,1);
        lcd.print("Door is Open");

        // Wait for door to be closed "delay"
        delay(5000);
        lcd.clear();
        menu = 0;
        hLocation = 1;
        vLocation = 0;
      }
      else{
        // Card scans and is bad
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Sorry");
  
        lcd.setCursor(0,1);
        lcd.print("Invalid Card");

        // Wait a little "delay"
        delay(5000);
        lcd.clear();
        menu = 0;
        hLocation = 1;
        vLocation = 0;
      }
      break;

    case 2:
      lcd.setCursor(0,0);
      lcd.print(sPage.Admin_Access[0]);
      lcd.setCursor(0,1);
      lcd.print(sPage.Admin_Access[1]);

      vLocation = vLocation + CheckVDirection();

      switch (vLocation){
        case -1:
          menu = 0;
          vLocation = 0;
          hLocation = 1;
          break;  
      }

//****// Finger Scanning    
      if (true){
        lcd.clear();
        menu = 3;
        vLocation = 0;
        hLocation = 1;
      }
      break;

    case 3:
      lcd.setCursor(0,0);
      lcd.print(sPage.Valid_Print[0]);  //  Access Granted
      
      hLocation = hLocation + CheckHDirection();

      switch (hLocation){
        case PAGES_VALID_PRINT:
          hLocation = 1;
          break;        

        case LOWEST_PAGE - 1:
          hLocation = PAGES_VALID_PRINT - 1;
          break;
      }

      switch (hLocation){
        case 1:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]); //  Enroll New Print
          break; 

        case 2:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]);  // Delete Existing Print
          break;

        case 3:
          if (cleared == false){
            lcd.setCursor (0, 1);
            lcd.print("                ");
            cleared = true; 
          }
          lcd.setCursor(0,1);
          lcd.print(sPage.Valid_Print[hLocation]);  //  Erase All Prints
          break;
      }
      
      vLocation = vLocation + CheckVDirection();
      
      switch (vLocation){
        case -1:
          lcd.clear();
          menu = 0;    //  Main Menu
          vLocation = 0;
          hLocation = 1;
          break;
          
        case 1:
          lcd.clear();
          menu = hLocation + 3;    //  Enroll New Print = 4, Delete Existing = 5, Erase All Prints = 6
          vLocation = 0;
          hLocation = 1;
          break;
      }
      break;

    case 4: //  Enroll New Print Menu
      lcd.setCursor(0,0);
      lcd.print(sPage.Enroll_Print[0]);  //  Enroll New Print
      lcd.setCursor(0,1);
      lcd.print(sPage.Enroll_Print[1]);  //  Print ID # 

      hLocation = hLocation + -CheckHDirection();
      
      switch (hLocation){
        case 0:
          hLocation = 1;
          break;

        case 6:
          hLocation = 5;
          break;
      }
      
      lcd.print("  "); lcd.print(hLocation);

      vLocation = CheckVDirection();

      switch (vLocation){
        case -1:
          lcd.clear();
          menu = 3;
          vLocation = 0;
          hLocation = 1;
          break;  

        case 1:
          vLocation = 0;
          hLocation = 1;
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Enroll_Print[2]);  //  Enter Finger

          // Scan Finger
          delay(2000);
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Enroll_Print[3]);  //  Remove Finger
          
          delay(2000);
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Enroll_Print[4]);  //  Enter Finger

          delay(2000);
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Enroll_Print[5]);  //  Finger Enrolled
          delay(2000);
          lcd.clear();
          menu = 0;
          // Add finger print counter
          break;
      }
      break;

    case 5:
      lcd.setCursor(0,0);
      lcd.print(sPage.Delete_Print[0]);  //  Delete Print
      lcd.setCursor(0,1);
      lcd.print(sPage.Delete_Print[1]);  //  Print ID #

      hLocation = hLocation + -CheckHDirection();
      
      switch (hLocation){
        case 0:
          hLocation = 1;
          break;

        case 6:
          hLocation = 5;
          break;
      }

      lcd.print("  "); lcd.print(hLocation);

      vLocation = CheckVDirection();

      switch (vLocation){
        case -1:
          lcd.clear();
          menu = 3;
          vLocation = 0;
          hLocation = 1;
          break;
        
        case 1:
          //  Delete Print
          delay(2000);
          lcd.setCursor (0, 1);
          lcd.print("                ");
          lcd.setCursor (0, 1);
          lcd.print(sPage.Delete_Print[2]); lcd.print(" #"); lcd.print(hLocation);
          delay(2000);

          lcd.clear();
          menu = 0;
          break;
      }
      break;

      //  
      case 6:
        lcd.setCursor(0,0);
        lcd.print(sPage.Delete_All[0]);  //  Delete All Prints
        lcd.setCursor(0,1);
        lcd.print(sPage.Delete_All[1]);  //  Are You Sure?
    
        vLocation = CheckVDirection();
  
        switch (vLocation){
          case -1:
            lcd.clear();
            menu = 3;
            vLocation = 0;
            hLocation = 1;
            break;
          
          case 1:
            //  Delete All The Prints
            lcd.setCursor (0, 1);
            lcd.print("                ");
            lcd.setCursor (0, 1);
            lcd.print("Deleting Prints");
            delay(2000);
            lcd.setCursor (0, 1);
            lcd.print("                ");
            lcd.setCursor (0, 1);
            lcd.print(sPage.Delete_All[2]);
            delay(2000);
  
            lcd.clear();
            menu = 0;
            vLocation = 0;
            hLocation = 1;
            break;
        }
  }
}

