#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
char arrays[] = " Aldrick        ";
char arrays1[] = " Castro         ";

void setup() {  
  lcd.init();
  lcd.backlight();
}

void loop() {
  /*for (int i = 0; i < 33; i++){
    //lcd.scrollDisplayLeft();
    if(i == 16){
      lcd.setCursor(0,1);
    }
    if (i < 16)
      lcd.print(arrays[i]); 
    else
      lcd.print(arrays1[i-16]);
    delay(100);
  }*/
  lcd.setCursor(0,0);
  lcd.print("Aldrick Castro &");
  lcd.setCursor(0,1);
  lcd.print("Jelitza Delgado");
  //lcd.clear();
 
}
