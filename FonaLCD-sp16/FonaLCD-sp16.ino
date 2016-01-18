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
  clearScreen();  
  delay(100);
  lcd.write(124);  // set brightness command
  lcd.write(157);   // set brightness 127 - 157 (brightest)
  delay(100);
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

  Serial.print(F("\n\rReading SMS #")); Serial.println(smsn);

  // Retrieve SMS sender address/phone number.
  if (! fona.getSMSSender(newSMS, replybuffer, 250)) {
    Serial.println("Failed!");
  }
  Serial.print(F("FROM: ")); Serial.println(replybuffer);

  // Retrieve SMS value.
  uint16_t smslen;
  if (! fona.readSMS(newSMS, replybuffer, 250, &smslen)) { // pass in buffer and max len!
    Serial.println("Failed!");
  }
  Serial.print(F("***** SMS #")); Serial.print(smsn);
  Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
  Serial.println(replybuffer);
  Serial.println(F("*****"));

  lcd.print(text);

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

void clearScreen(){
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
}






