/*
  dollbot_anchor_test: Anchor dollbot in a specific heading

  Control the dollbot via the remote control interface, once control is released the dollbot will stay anchored in the same orientation using its built-in
  magnetic compass.
  
  http://splashbots.blogspot.com/
  
  Based on Sparkfun's "RC Hobby Controllers and Arduino": https://www.sparkfun.com/tutorials/348
  Requires Adafruit MotorShield v2: http://www.adafruit.com/products/1438
  Requires Adafruit HMC5883 and Adafruit sensor library https://learn.adafruit.com/adafruit-hmc5883l-breakout-triple-axis-magnetometer-compass-sensor/

*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#undef DEBUG
//#define DEBUG

#define PIN_CH1 7
#define PIN_CH2 6
#define PIN_CH3 5
#define PIN_CH4 4

#define MOTOR_FL 1
#define MOTOR_FR 2
#define MOTOR_BL 3
#define MOTOR_BR 4

#define PWM_LOW 1000
#define PWM_HIGH 2000

#define RC_THRESHOLD 10 // Minimum value of an RC input to move to manual control
#define MOTOR_THRESHOLD 5 // Minimum value of a motor input to spin, smaller than this is considered noise

#define MOTOR_MAX 255

void vehicleMove(int valX, int valY, int valRotate);
void vehicleStop();
void HolonomicMove(int xComponent, int yComponent, int rotationComponent);
void HolonomicSpin(int mm[4]);
void HolonomicSimplify(int mm[4]);
int rotationalDeviation(int initial, int target);

// Holonomic matrix - connect all motors to the motor shield in the same position. If movement is not as expected change the connections (do not change the table below).

int verticalMove[4] =   {-1, -1, -1, -1};
int horizontalMove[4] = {-1, +1, +1, -1};
int rotationMove[4] =   {-1, +1, -1, +1};

// Control variables
int manualControl;
float anchorHeading = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motor[4] = {AFMS.getMotor(MOTOR_FL), AFMS.getMotor(MOTOR_FR), AFMS.getMotor(MOTOR_BL), AFMS.getMotor(MOTOR_BR)};

// Initialise the compass
Adafruit_HMC5883_Unified compass = Adafruit_HMC5883_Unified(8431);

void setup() {
  // Initialize serial communications
  Serial.begin(9600);
  
  Serial.println("Initializing input...");

  AFMS.begin();  // create with the default frequency 1.6KHz

  /* Initialise the compass */
  if(!compass.begin())
  {
    Serial.println("HMC5883 compass not detected, stopping.");
    while(1);
  }

  // Initialize the ports where we will read receiver input
  pinMode(PIN_CH1, INPUT);
  pinMode(PIN_CH2, INPUT);
  pinMode(PIN_CH3, INPUT);
  pinMode(PIN_CH4, INPUT);
  
  // Automated control initialisation
  manualControl = 1;
  
#ifdef DEBUG
  Serial.println("DEBUG mode enabled");
#endif

  Serial.println("Initialization completed");
}

void dumpContent(int mm[4]) {
  Serial.print("MM values: ");
  for(int i=0; i < 4; i++) {
    Serial.print(mm[i]);
    Serial.print(",");
  }
  Serial.println("");
}

void HolonomicSpin(int mm[4]) {
  HolonomicSimplify(&mm);
  
#ifdef DEBUG
  dumpContent(mm);
#endif

  for(int i = 0; i < 4; i++) {
    
    // Expand range to motor limits
    mm[i] = map(mm[i], -100, 100, -1 * MOTOR_MAX, MOTOR_MAX);
    
    if(abs(mm[i]) > (MOTOR_THRESHOLD * MOTOR_MAX) / 100) {
      motor[i]->setSpeed(abs(mm[i]));
      motor[i]->run(mm[i] > 0? FORWARD : BACKWARD);
#ifdef DEBUG
      Serial.print("Spinning motor #");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(mm[i]);
#endif
    } else {
      motor[i]->setSpeed(0);
      motor[i]->run(RELEASE);
#ifdef DEBUG
      Serial.print("Stopping motor #");
      Serial.println(i);
#endif
    }
  }
}

void HolonomicSimplify(int **mm) {
  int max = 0;

  for(int i = 0; i < 4; i++) {
    int currentValue = abs(*mm[i]);
    
    if(currentValue > max) {
      max = currentValue;
    }
  }
  
  // Scale the values so that we stay within the -100 .. 100 range

  if(max > 100) {
    for(int i = 0; i < 4; i++) {
      *mm[i] = map(*mm[i], -1 * max, max, -100, 100);
    }
  }

}

void HolonomicMove(int xComponent, int yComponent, int rotationComponent) {
  int mm[4] = {0, 0, 0, 0};
  
  if(xComponent) {
    for(int i = 0; i < 4; i++) {
      mm[i] += horizontalMove[i] * xComponent;
    }
  }

  if(yComponent) {
    for(int i = 0; i < 4; i++) {
      mm[i] += verticalMove[i] * yComponent;
    }
  }
  
  if(rotationComponent) {
    for(int i = 0; i < 4; i++) {
      mm[i] += rotationMove[i] * rotationComponent;
    }
  }
  
  HolonomicSpin(mm);
}


/*
  Helper function to move the vehicle 
 
*/

void vehicleMove(int valX, int valY, int valRotate) {
  HolonomicMove(valX, valY, valRotate);
}

void vehicleStop() {
  vehicleMove(0, 0, 0);
}

// From: http://gmc.yoyogames.com/index.php?showtopic=532420

int rotationalDeviation(int initial, int target) {
  return (((target - initial) % 360) + 360 + 180) % 360 - 180;
}

void loop() {
  sensors_event_t compassEvent;
  float heading;

  // Read three of the channels of the RC receiver, we only use three - but you can add a fourth one if needed

  int channelHorizontal = pulseIn(PIN_CH4, HIGH, 25000); // X axis movement
  int channelVertical = pulseIn(PIN_CH2, HIGH, 25000); // Y axis movement
  int channelRotate = pulseIn(PIN_CH1, HIGH, 25000); // Rotation

  int directionHorizontal = map(channelHorizontal, PWM_LOW, PWM_HIGH, -100, 100);
  int directionVertical = map(channelVertical, PWM_LOW, PWM_HIGH, -100, 100);
  int angleRotate = map(channelRotate, PWM_LOW, PWM_HIGH, -100, 100);
  
  // If we are receiving control commands from the RC controller bypass autonomous mode

  if(abs(directionHorizontal) > RC_THRESHOLD || abs(directionVertical) > RC_THRESHOLD || abs(angleRotate) > RC_THRESHOLD) {
    
    manualControl = 1;
    
    // Map all readings to a -100 .. +100 range so it is easy to deal with


#ifdef DEBUG    
    Serial.print("MANUAL: RC control ");

    Serial.print("H: ");
    Serial.print(directionHorizontal);

    Serial.print(" V: ");
    Serial.print(directionVertical);

    Serial.print(" R: ");
    Serial.println(angleRotate);
#endif

    vehicleMove(directionHorizontal, directionVertical, angleRotate);
    
  } else {
    // Autonomous control
    compass.getEvent(&compassEvent);
    heading = int(atan2(compassEvent.magnetic.y, compassEvent.magnetic.x) * 180/M_PI + 360) % 360; // Convert radians to degrees
    
    if(manualControl == 1) {
      manualControl = 0;
      // We just came here from being controlled by the RC. Stop all engines and get our anchor heading

      vehicleStop();

      anchorHeading = heading;
#ifdef DEBUG
      Serial.print("AUTONOMOUS: Set new anchor heading: ");
      Serial.println(anchorHeading);
#endif

    } else {
      // Get our current heading and correct it so we remain close to anchor
      int deviation = rotationalDeviation(anchorHeading, heading);

      if(abs(deviation) > 1) {
#ifdef DEBUG
        Serial.print("AUTONOMOUS: Correcting heading by ");
        Serial.println(deviation);
#endif
        vehicleMove(0, 0, deviation);
      } else {
        vehicleStop();

#ifdef DEBUG
        Serial.print("AUTONOMOUS: Not moving, only a small deviation: ");
        Serial.println(deviation);
#endif
      }
      
    }
  }

#ifdef DEBUG  
 Serial.println("Debug sleeping");
 delay(500);
#endif
}

