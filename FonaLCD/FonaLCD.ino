//based on Adafruit FONA code, with sections from "Building Wireless Sensor Networks" by Robert Faludi.
//Synthesized by Andrew Kleindolph 2015
//For use with Adafruit Fona and Sparkfun Serial LCD

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

#define WIDTH 16

// this is a large buffer for replies
char replybuffer[255];

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

SoftwareSerial lcd(9, 8);

int newSMS;

void setup() {
  Serial.begin(9600);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  lcd.begin(9600);  // start lcd serial
  fonaSS.begin(4800);

  // See if the FONA is responding
  if (! fona.begin(fonaSS)) {  // make it slow so its easy to read!
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[15] = {
    0  }; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); 
    Serial.println(imei);
  }

  lcd.write(254);  // set cursor command
  lcd.write(128);  //Row 0 position 0
  lcd.print("text me now!!!!");
  delay(1000);
}
void loop() {
  unsigned long startTime = millis();
  String text = replybuffer;
  // read the number of SMS's!
  int8_t smsnum = fona.getNumSMS();
  if (smsnum < 0) {
    Serial.println(F("Could not read # SMS"));
  } 
  else {
    Serial.print(smsnum); 
    Serial.println(F(" SMS's on SIM card!"));
  }

  if (smsnum > 29) {
    for (int i = 0; i < 31; i++) {
      fona.deleteSMS(i);
      delay(50);
    }
    lcd.write(254);  // set cursor command
    lcd.write(128);  //Row 0 position 0
    lcd.print ("Clearing Memory");
    lcd.write(254);  // set cursor command
    lcd.write(192);  //Row 0 position 0
    lcd.print ("Please Resend");
    delay(3000);
    clearScreen();
  }
  uint8_t smsn = smsnum;

  if (newSMS < smsnum || smsnum == 0) {
    newSMS = smsnum;
    Serial.println("newSMS");
  }

  Serial.print(F("\n\rReading SMS #")); 
  Serial.println(smsn);

  // Retrieve SMS sender address/phone number.
  if (! fona.getSMSSender(3, replybuffer, 250)) {
    Serial.println("Failed!");
  }
  Serial.print(F("FROM: ")); 
  Serial.println(replybuffer);

  // Retrieve SMS value.
  uint16_t smslen;
  if (! fona.readSMS(smsn, replybuffer, 250, &smslen)) { // pass in buffer and max len!
    Serial.println("Failed!");
  }
  Serial.print(F("***** SMS #")); 
  Serial.print(smsn);
  Serial.print(" ("); 
  Serial.print(smslen); 
  Serial.println(F(") bytes *****"));

  Serial.println(replybuffer);

  //filter out for emoticons
  if  ((replybuffer[0] == 'D') && (replybuffer[1] == '8') && (replybuffer[2] == '3') && (replybuffer[3] == 'D')) {
    invalid_message();
  }

  if  ((replybuffer[0] == 'D') && (replybuffer[1] == '8') && (replybuffer[2] == '3') && (replybuffer[3] == 'C')) {
    invalid_message();
  }

  if (text.length() > 1) {
    lcd.write(254);  // set cursor command
    lcd.write(128);  //Row 0 position 0
    if (text.length() > 0) {
      unsigned long displayTime = 800; //300000 = 5 minutes
      while (millis() - startTime < displayTime) {
        clearScreen();
        showText(text);
        // pause after showing the string
        delay(700);
      }
    }
  }
}

void flushSerial() {
  while (Serial.available()) 
    Serial.read();
}

char readBlocking() {
  while (!Serial.available());
  return Serial.read();
}
uint16_t readnumber() {
  uint16_t x = 0;
  char c;
  while (! isdigit(c = readBlocking())) {
    //Serial.print(c);
  }
  Serial.print(c);
  x = c - '0';
  while (isdigit(c = readBlocking())) {
    Serial.print(c);
    x *= 10;
    x += c - '0';
  }
  return x;
}

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;

  while (true) {
    if (buffidx > maxbuff) {
      //Serial.println(F("SPACE"));
      break;
    }

    while(Serial.available()) {
      char c =  Serial.read();

      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;

        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }

    if (timeoutvalid && timeout == 0) {
      //Serial.println(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}

//-----function below properly displays text-----
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

void invalid_message() {
  lcd.write(254);  //set cursor command
  lcd.write(128);  //Row 1 position 0
  lcd.print("no emoticons");
  lcd.write(254);  //set cursor command
  lcd.write(192);  //Row 1 position 0
  lcd.print("please!");
  delay(2000);
  loop();
}






