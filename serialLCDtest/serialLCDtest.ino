#include <SoftwareSerial.h>

SoftwareSerial lcd(2, 8);

void setup()
{
  Serial.begin(9600);  // start serial monitor serial
  lcd.begin(9600);  // start lcd serial
}

void loop()
{
  lcd.print("hello all yall!");
  delay(1000);
  clearScreen();
  lcd.print("welcome to the farm!");
  delay(1000);
  clearScreen();
 
}

void clearScreen(){
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
  lcd.write(0xFE);  // send the special command
  lcd.write(0x80);  // send the set cursor command
}



