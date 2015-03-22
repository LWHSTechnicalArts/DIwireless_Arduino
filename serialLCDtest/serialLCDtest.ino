#include <SoftwareSerial.h>
SoftwareSerial lcd(2, 8);

void setup()
{
  lcd.begin(9600);  // start lcd serial
}

void loop()
{
  lcd.write(254);  // set cursor command
  lcd.write(128);  //Row 0 position 0
  lcd.print("hello all yall!");
  delay(1000);
    
  lcd.write(254); // set cursor command
  lcd.write(192); //Row 1 position 0
  lcd.print("catch a chicken!");
  delay(1000);
  
  clearScreen();
  delay(1000);
}

void clearScreen(){
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
}





