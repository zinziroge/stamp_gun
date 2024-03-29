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


#define TRIGGER_SW_PIN      5   // pull up
#define DET_ROULETTE_SW_1   18  // pull up
#define DET_ROULETTE_SW_2   19  // pull up
#define SERVO_ROULETTE_PIN  15  // 360 deg servo motor. 端っこ
#define SERVO_PUSHER_PIN    4   // 0deg <= angle <= 180deg
#define PWM_ROULETTE_CH     0
#define PWM_PUSHER_CH       1

// SG90, 50Hz, 0.5ms ~ 2.4ms
#define SERVO_PUSHER_MIN    26  // (26/1024)*20ms = 0.5ms (-90deg)
#define SERVO_PUSHER_MAX    123 // (123/1024)*20ms = 2.4ms (+90deg)
#define SERVO_PUSHER_STOP   78  // 

/*
 * FS90R
 *   700us <= pulse_width <= 2300us 
 *   1500us stop
 *   
 *   0.7ms = x/1024 * 20ms -> x = 36 
 *   2.3ms = x/1024 * 20ms -> x = 118  
 *   1.5ms = x/1024 * 20ms -> x = 77 (77-36=41, 118 - 77 = 41)
 */
#define SERVO_ROULETTE_MIN  36
#define SERVO_ROULETTE_STOP  (77 + 2)
#define SERVO_ROULETTE_MAX  118
#define SERVO_ROULETTE_ROT  (SERVO_ROULETTE_STOP+5)

int potVal;  // variable to read the value from the analog pin
int angle;   // variable to hold the angle for the servo motor

/***********************************************************************/
void setup() {
  pinMode(TRIGGER_SW_PIN, INPUT);
  pinMode(DET_ROULETTE_SW_1, INPUT);
  pinMode(DET_ROULETTE_SW_2, INPUT);
  pinMode(SERVO_ROULETTE_PIN, OUTPUT);
  pinMode(SERVO_PUSHER_PIN, OUTPUT);

  ledcSetup(PWM_ROULETTE_CH, 50, 10);  // 0ch 50 Hz 10bit resolution
  ledcAttachPin(SERVO_ROULETTE_PIN, PWM_ROULETTE_CH); // 15pin, 0ch
  ledcSetup(PWM_PUSHER_CH, 50, 10);  // 1ch 50 Hz 10bit resolution
  ledcAttachPin(SERVO_PUSHER_PIN, PWM_PUSHER_CH); // 15pin, 0ch
  
  ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MIN); // ready
  
  Serial.begin(115200);
  Serial.println("start");
}

int32_t is_stamp_ready_pushed(void) {
  return (digitalRead(DET_ROULETTE_SW_1)==0 && digitalRead(DET_ROULETTE_SW_2)==0);
}

void init_roulette(void) {
  uint32_t t = millis();
  
  ledcWrite(SERVO_ROULETTE_PIN, 0);
  while(!is_stamp_ready_pushed() && (millis() - t < 3000)) {
     delay(1);
  }
  ledcWrite(SERVO_ROULETTE_PIN, 0);

}

int n = SERVO_PUSHER_MIN;
int m = 0;

void test_sw(void) {
  Serial.print(digitalRead(DET_ROULETTE_SW_1));  
  Serial.print(", ");
  Serial.print(digitalRead(DET_ROULETTE_SW_2));
  Serial.print(", ");  
  Serial.print(digitalRead(TRIGGER_SW_PIN));
  Serial.println();  
}

void test_servo_motor_2(void) {
  while(Serial.available() > 0) {
    int recv_byte = Serial.read();
    Serial.print("recieved, ");
    Serial.println(recv_byte, DEC);
  
    if(recv_byte == 'j') {
      ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MIN);
    } else if(recv_byte == 'k') {
      ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_STOP);
    } else if(recv_byte == 'l') {
      ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MAX);
    } else if(recv_byte == 'a') {
      ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_MIN);
    } else if(recv_byte == 's') {
      ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_STOP);
    } else if(recv_byte == 'd') {
      ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_MAX);
    } else if(recv_byte == 'f') {
      ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_ROT);
    } else if(recv_byte == 'q') {
      ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_STOP);
      ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_STOP);
    }
  }

  Serial.print(digitalRead(DET_ROULETTE_SW_1));  
  Serial.print(", ");
  Serial.print(digitalRead(DET_ROULETTE_SW_2));
  Serial.print(", ");  
  Serial.println();
  
  delay(100);
}

void test_servo_motor_1(void) {
    ledcWrite(PWM_PUSHER_CH, 0);
    delay(1000);
    ledcWrite(PWM_PUSHER_CH, 1023);
    delay(1000);

    ledcWrite(PWM_ROULETTE_CH, 1023);
    delay(1000);
}

void test_trigger(void) {
  static int pre_trigger_sw = 1;
  
  Serial.print(digitalRead(TRIGGER_SW_PIN));
  Serial.println(); 

  int cur_trigger_sw = digitalRead(TRIGGER_SW_PIN);
  //if((pre_trigger_sw == 1) && (cur_trigger_sw == 0)) { // fall edge
  if(cur_trigger_sw == 0) { // while pressing
    Serial.println("Trigger pins was pressed.");
    /*
    // pusher
    ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MAX); // push
    delay(2000);
    ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MIN); // ready
    */
    // roulette
    ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_MAX);
    delay(120);
    ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_STOP);
  }

  pre_trigger_sw = cur_trigger_sw;
}

void push_rot_only(void) {
  static int pre_trigger_sw = 1;
  int cur_trigger_sw = digitalRead(TRIGGER_SW_PIN);

  Serial.print(digitalRead(TRIGGER_SW_PIN));
  Serial.println();  
    
  if((pre_trigger_sw == 1) && (cur_trigger_sw == 0)) { // fall edge
  //if(cur_trigger_sw == 0) { // while pressing
    // pusher
    ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_ROT);
    ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MAX); // push
    delay(1000^- 0);

    ledcWrite(PWM_ROULETTE_CH, SERVO_ROULETTE_STOP);
    ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MIN); // ready
  }

  pre_trigger_sw = cur_trigger_sw;
  ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MIN); 
}

void push_only(void) {
  static int pre_trigger_sw = 1;
  int cur_trigger_sw = digitalRead(TRIGGER_SW_PIN);
  
  if((pre_trigger_sw == 1) && (cur_trigger_sw == 0)) { // fall edge
  //if(cur_trigger_sw == 0) { // while pressing
    // pusher
    ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MAX); // push
    delay(1000);
    ledcWrite(PWM_PUSHER_CH, SERVO_PUSHER_MIN); // ready
  }

  pre_trigger_sw = cur_trigger_sw;
}

void loop(void) {
  //push_rot_only();
  push_only();
  //test_sw();
  //test_servo_motor_2();
  //test_trigger();
}

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
#endif
