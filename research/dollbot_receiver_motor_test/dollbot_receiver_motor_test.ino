/*
  dollbot_receiver_motor_test: Control a motor using the 4-CH RC receiver.
  
  This sketch polls three of the channels of the RC receiver and spins a motor when one of the channels is working. Designed to test the
  Adafruit motor shield with the RC motor and the motor casing as part of leak testing.
  
  http://splashbots.blogspot.com/
  
  Based on Sparkfun's "RC Hobby Controllers and Arduino": https://www.sparkfun.com/tutorials/348
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define DEBUG

#define PIN_CH1 7
#define PIN_CH2 6
#define PIN_CH3 5
#define PIN_CH4 4

#define TEST_MOTOR 4

#define PWM_LOW 1000
#define PWM_HIGH 2000
#define RC_THRESHOLD 10

#define AXIS_X 0
#define AXIS_Y 1

#define MOTOR_MAX 255

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *testMotor= AFMS.getMotor(TEST_MOTOR);

void setup() {
  // Initialize serial communications
  Serial.begin(9600);
  Serial.println("Initializing input...");
  
  AFMS.begin();  // create with the default frequency 1.6KHz

  // Initialize the ports where we will read receiver input
  pinMode(PIN_CH1, INPUT);
  pinMode(PIN_CH2, INPUT);
  pinMode(PIN_CH3, INPUT);
  pinMode(PIN_CH4, INPUT);

}

void HolonomicMove(int xComponent, int yComponent) {
  if(xComponent) {
    if(xComponent < -10) {
      int sp = (abs(xComponent) * MOTOR_MAX) / 100;
      Serial.print("Setting backward speed to: ");
      Serial.println(sp);
      testMotor->setSpeed(sp); 
      testMotor->run(BACKWARD);
    } else if (xComponent > 10) {
      int sp = (abs(xComponent) * MOTOR_MAX) / 100;
      Serial.print("Setting forward speed to: ");
      Serial.println(sp);
      testMotor->setSpeed(sp); 
      testMotor->run(FORWARD);
    } else {
      Serial.println("Releasing");
      testMotor->run(RELEASE);
    }
  }
}

/*
  Helper function to move the vehicle - or in the current incarnation spin up a motor instead
*/

void vehicleMove(int axis, int val) {
  switch(axis) {
    case AXIS_X:
      HolonomicMove(val, 0);
      break;

    case AXIS_Y:
      HolonomicMove(0, val);
      break;
  }
}

/*
  Helper function to rotate the vehicle - or in the current incarnation spin up a motor instead
*/

void vehicleRotate(int val) {

  /*
  if(val < -10) {
    digitalWrite(LED_TURN_RIGHT, LOW);    
    digitalWrite(LED_TURN_LEFT, HIGH);
    Serial.println("Lighting LEFT");
  } else if (val > 10) {
    digitalWrite(LED_TURN_LEFT, LOW);    
    digitalWrite(LED_TURN_RIGHT, HIGH);
    Serial.println("Lighting RIGHT");
  } else {
    digitalWrite(LED_TURN_LEFT, LOW);    
    digitalWrite(LED_TURN_RIGHT, LOW);
    Serial.println("Lighting OFF");
  }
  */
  
}

void loop() {
  // Read three of the channels of the RC receiver, we only use three - but you can add a fourth one if needed

  int channelHorizontal = pulseIn(PIN_CH4, HIGH, 25000); // X avis movement
  int channelVertical = pulseIn(PIN_CH2, HIGH, 25000); // Y axis movement
  int channelRotate = pulseIn(PIN_CH1, HIGH, 25000); // Rotation
  
  int directionHorizontal = map(channelHorizontal, PWM_LOW, PWM_HIGH, -100, 100);
  int directionVertical = map(channelVertical, PWM_LOW, PWM_HIGH, -100, 100);
  int angleRotate = map(channelRotate, PWM_LOW, PWM_HIGH, -100, 100);
  
  // If we are receiving control commands from the RC controller bypass autonomous mode

  if(abs(directionHorizontal) > RC_THRESHOLD || abs(directionVertical) > RC_THRESHOLD || abs(angleRotate) > RC_THRESHOLD) {
    
    // Map all readings to a -100 .. +100 range so it is easy to deal with


#ifdef DEBUG    
    Serial.print("Manual control ");

    Serial.print("H: ");
    Serial.print(directionHorizontal);

    Serial.print(" V: ");
    Serial.print(directionVertical);

    Serial.print(" R: ");
    Serial.println(angleRotate);
#endif

    vehicleMove(AXIS_X, directionHorizontal);
    vehicleMove(AXIS_Y, directionVertical);
    vehicleRotate(angleRotate);
    
  } else {
    Serial.println("Autonomous control");
    vehicleMove(AXIS_X, 0);
    vehicleMove(AXIS_Y, 0);
    vehicleRotate(0);
  }

#ifdef DEBUG  
 delay(500);
#endif
}

