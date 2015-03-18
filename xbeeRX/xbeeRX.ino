#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  // initialize the LED pins as outputs:
  pinMode(8, OUTPUT);
}
void loop() {
  // if there's any serial data in the buffer, read a byte:
  if (mySerial.available() > 0) {
    int inByte = mySerial.read(); 
    Serial.print(inByte);
    
    if (inByte == 'H') {
      digitalWrite(8,HIGH); 
    }
    if (inByte == 'L') {
      digitalWrite(8,LOW); 
    }
  }
}



