/*
  dollbot_motor_test: Test the four motors in both directions.
  
  This sketch sequentially powers up and down the four motors in dollbot. Use it to verify correct installation of the thrusters, motors, motor shield and batteries.
  
  http://splashbots.blogspot.com/
  
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define DEBUG

#define MOTOR_MAX 255
#define TEST_DELAY 1000

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

void setup() {
  // Initialize serial communications
  Serial.begin(9600);
  Serial.println("Initializing input...");
  
  AFMS.begin();  // create with the default frequency 1.6KHz
}

void motorTest(int motorNumber) {
 Adafruit_DCMotor *motor = AFMS.getMotor(motorNumber);

#ifdef DEBUG
 Serial.print("Testing motor #");
 Serial.print(motorNumber);
 Serial.print("\n");
#endif

 motor->setSpeed(MOTOR_MAX / 2);
 motor->run(BACKWARD);
 delay(TEST_DELAY);
 motor->run(FORWARD);
 delay(TEST_DELAY);
 motor->run(RELEASE);
 delay(TEST_DELAY);

}

void loop() {
  
    for (int i=1; i <= 4; i++) {
        motorTest(i);
    }

 delay(2 * TEST_DELAY);
}

