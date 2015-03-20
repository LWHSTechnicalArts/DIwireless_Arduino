#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(&fonaSS, FONA_RST);

void setup() {
  pinMode(7, INPUT);
  
  Serial.begin(115200);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));


  // See if the FONA is responding
  if (! fona.begin(4800)) {  // make it slow so its easy to read!
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); 
    Serial.println(imei);
  }
}

void loop() {
  int buttonState=digitalRead(7);     //test button state
  if (buttonState==HIGH){

    fona.sendSMS("14150000000", "!!!!The Button is Pressed!!!!!");  //phone number and message here 140 characters max
    Serial.println("Sent!");
    delay(5000);
  }
  delay(200);
}



