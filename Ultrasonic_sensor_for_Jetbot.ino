/*
Ultrasonic sensor for Jetbot with Arduino Nano
----------------------------
HC-SR04
Trigger on port 11
Respons on port 12
Vcc <=> Arduino Nano 3V3
I2C slave sender on port 8 (Arduino Nano pin: A4 = SDA; A5 = SCL)
----------------------------
Arduino Nano is powered by Jetson Nano: pin 5V <=> 5v ; GND <=> GND
----------------------------
The original source of Ping))) can be found on:
  http://www.arduino.cc/en/Tutorial/Ping
This example code is in the public domain.
*/

#include <Wire.h>

// this constant won't change. 
// It's the pin number of the sensor's output:
const int pingPin = 11;
const int distPin = 12;
long cm;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(pingPin, OUTPUT);
  pinMode(distPin, INPUT);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches;
  // The PING))) is triggered by a HIGH pulse of 2 
  // or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean 
  // HIGH pulse:
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  // A HIGH pulse whose duration is the time (in 
  // microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  duration = pulseIn(distPin, HIGH);
  // convert the time into a distance
  // inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  // Print distance to serial monitor for testing
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
}

// I2C function that executes whenever data is requested 
// by master. This function is registered as an event, 
// see setup()
void requestEvent() {
  byte buf[4];
  buf[0] = (byte) cm;
  buf[1] = (byte) cm>>8;
  buf[2] = (byte) cm>>16;
  buf[3] = (byte) cm>>24;
  Wire.write(buf, 4); // respond with message of 4 bytes 
                      // (cm is long type)
  Serial.println("I2C request");
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), 
  // there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 
  // feet per second).
  // This gives the distance travelled by the ping, 
  // outbound and return,
  // so we divide by 2 to get the distance of the 
  // obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds 
  // per centimeter.
  // The ping travels out and back, so to find the 
  // distance of the object we take half of the distance 
  // travelled.
  return microseconds / 29 / 2;
}
