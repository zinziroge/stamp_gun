/*
  Arduino Starter Kit example
  Project 5 - Servo Mood Indicator

  This sketch is written to accompany Project 5 in the Arduino Starter Kit

  Parts required:
  - servo motor
  - 10 kilohm potentiometer
  - two 100 uF electrolytic capacitors

  created 13 Sep 2012
  by Scott Fitzgerald

  http://www.arduino.cc/starterKit

  This example code is part of the public domain.
*/


int potVal;  // variable to read the value from the analog pin
int angle;   // variable to hold the angle for the servo motor

#if 0
// include the Servo library
//#include <Servo.h>
//Servo myServo;  // create a servo object
//int const potPin = A0; // analog pin used to connect the potentiometer

void setup_uno() {
  myServo.attach(9); // attaches the servo on pin 9 to the servo object
  Serial.begin(9600); // open a serial connection to your computer
}

void loop_uno() {
  potVal = 0;
  
  // scale the numbers from the pot
  angle = map(potVal, 0, 1023, 0, 179);

  // set the servo position
  //myServo.write(angle);

  // wait for the servo to get there
  delay(3000);
  
  ///////////////////////////////////////////////////////
  potVal = 128;

  // scale the numbers from the pot
  angle = map(potVal, 0, 1023, 0, 179);
  Serial.println(angle);
  // set the servo position
  myServo.write(85);

  // wait for the servo to get there
  delay(3000);
}
#endif

void setup() {
  pinMode(15, OUTPUT);
  ledcSetup(0, 50, 10);  // 0ch 50 Hz 10bit resolution
  ledcAttachPin(15, 0); // 15pin, 0ch
  Serial.begin(115200);
  Serial.println("start");
}

int _min = 26;  // (26/1024)*20ms ≒ 0.5 ms  (-90°)
int _max = 123; // (123/1024)*20ms ≒ 2.4 ms (+90°)
int n = _min;
void loop() {
  ledcWrite(0, n);
  n += 5;
  if (n > _max) 
    n = _min;
  delay(500);
}

