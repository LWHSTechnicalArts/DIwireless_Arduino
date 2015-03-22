#include <SoftwareSerial.h>
SoftwareSerial lcd(2, 8);
#define WIDTH 16 //LED line width

void setup()
{
  Serial.begin(9600);  // start serial monitor serial
  lcd.begin(9600);  // start lcd serial

  clearScreen();  

  lcd.write(124);  // set brightness command
  lcd.write(157);   // set brightness 127 - 157 (brightest)

}

void loop()
{
  String text = ("I'm feeling really good about writing with this LCD. It has a smooth output!");
  showText(text);
}

void showText(String text) {
  int cpos = 0; // keeps track of the current cursor position
  int line = 0; // keeps track of the current line
  
  for (int i = 0; i < text.length(); i++) {   // step through the text one character at a time
    boolean linefeed = false;     // in general, don't make a linefeed
    if (text[i] == ' ') {    // if the current character is a space, then make a line feed
      linefeed = true;
      // ...but check first that there isn't another space before the edge of the screen
      for (int j = i + 1; j < i + WIDTH - cpos + 1 && j < text.length() ; j++) {
        if (text[j] == ' ') linefeed = false; // another space before the edge of the screen
        else if (j == text.length() - 1) linefeed = false; // all of the text completes before the edge of the screen
      }
    }
    
    // make a linefeed if we reach the edge of the screen (if a word is greater in length than the width)
    if (cpos >= WIDTH) {
      linefeed == true;
    }

    // on linefeeds
    if (linefeed == true) {
      switch (line) {
      case 0:
        lcd.write(254);  //set cursor command
        lcd.write(192);  //Row 1 position 0
        line = 1;
        break;
      case 1:
        delay(500);     //delay before displaying next section
        clearScreen();
        lcd.write(254); //set cursor command
        lcd.write(128); //Row 0 position 0
        line = 0;
        break;
      }
      cpos = 0; // reset the cursor tracker to the beginning of the screen
    }

    // if this isn't a line feed
    else {
      // print the current character, add it to the line buffer and advance the cursor position
      lcd.print(text[i]);
      cpos++;
      delay(50); // wait a moment after each character
    }
  }
  delay(1000);  //one second delay at end of message
  clearScreen();
}

void clearScreen(){
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
}




