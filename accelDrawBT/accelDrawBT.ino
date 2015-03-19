#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();


int firstSensor = 0;    // first analog sensor
int secondSensor = 0;   // second analog sensor
int thirdSensor = 0;    // digital sensor

void setup() {
  // configure the serial connection:
  Serial.begin(9600);

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }

  mma.setRange(MMA8451_RANGE_2_G);

}

void loop() {
  mma.read();
  firstSensor = map(mma.x,-4200,4200,0,254);
  secondSensor = map(mma.y,-4200,4200,0,254);
  thirdSensor = map(mma.z,-4200,4200,0,254);
  Serial.print(firstSensor);
  Serial.write(44);
  Serial.print(secondSensor);
  Serial.write(44);
  Serial.print(thirdSensor);
  Serial.write(10);
  //  Serial.print(firstSensor);
  //  Serial.print(",");
  //  Serial.print(secondSensor);
  //  Serial.print(",");
  //  Serial.println(thirdSensor);
  delay(5);
}




