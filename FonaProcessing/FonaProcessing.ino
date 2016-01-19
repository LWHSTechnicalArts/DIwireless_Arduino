//based on Adafruit FONA code, with sections from "Building Wireless Sensor Networks" by Robert Faludi.
//Synthesized by Andrew Kleindolph 2015
//For use with Adafruit Fona and Processing
int inByte = 0;

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

// this is a large buffer for replies
char replybuffer[255];

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

int newSMS;

void setup() {
  Serial.begin(9600);
  fonaSS.begin(4800);
  //  Serial.println(F("FONA basic test"));
  //  Serial.println(F("Initializing....(May take 3 seconds)"));

  // See if the FONA is responding
  if (! fona.begin(fonaSS)) {  
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[15] = {
    0  }; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
}
void loop() {
  unsigned long startTime = millis();
  String text = replybuffer;
  // read the number of SMS's!
  int8_t smsnum = fona.getNumSMS();

  if (smsnum < 0) {
    Serial.println(F("Could not read # SMS"));
  } 

  if (smsnum > 29) {
    for (int i = 0; i < 31; i++) {
      fona.deleteSMS(i);
      delay(50);
    }

    Serial.print ("Clearing Memory");
  }
  uint8_t smsn = smsnum;

  if (newSMS < smsnum || smsnum == 0) {
    newSMS = smsnum;
  }


  // Retrieve SMS sender address/phone number.
  if (! fona.getSMSSender(3, replybuffer, 250)) {
    Serial.println("Failed!");
  }

  // Retrieve SMS value.
  uint16_t smslen;
  if (! fona.readSMS(smsn, replybuffer, 250, &smslen)) { // pass in buffer and max len!
    Serial.println("Failed!");
  }
     Serial.println(replybuffer);
 
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








