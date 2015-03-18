#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX


void setup()  {
  mySerial.begin(9600);
  pinMode(4,OUTPUT);

}

void loop()
{
  int buttonState=digitalRead(4);
  if (buttonState==HIGH){
    mySerial.print('H');
    delay(100);
  }
  mySerial.print('L');
  delay(100);
}



