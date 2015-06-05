/*
  dollbot_receiver_test: Control positional LEDs with a 4-CH RC receiver.
  
  This sketch polls three of the channels of the RC receiver and lits up a set of 6 leds accordingly, one for each channel and direction. It 
  is the initial test for a RC controlled water bot, you can find more information on the dollbot project page:
  
  http://splashbots.blogspot.com/
  
  Based on Sparkfun's "RC Hobby Controllers and Arduino": https://www.sparkfun.com/tutorials/348
*/

// #define DEBUG

#define PIN_CH1 7
#define PIN_CH2 6
#define PIN_CH3 5
#define PIN_CH4 4

#define LED_TURN_LEFT A0
#define LED_TURN_RIGHT A1
#define LED_FORWARD A2
#define LED_BACKWARDS A3
#define LED_LEFT A4
#define LED_RIGHT A5

#define PWM_LOW 1000
#define PWM_HIGH 2000

#define AXIS_X 0
#define AXIS_Y 1

void setup() {
  // Initialize serial communications
  Serial.begin(9600);
  Serial.println("Initializing input...");
  
  // Initialize the ports where we will read receiver input
  pinMode(PIN_CH1, INPUT);
  pinMode(PIN_CH2, INPUT);
  pinMode(PIN_CH3, INPUT);
  pinMode(PIN_CH4, INPUT);

  // Initialize the LED output ports
  pinMode(LED_TURN_LEFT, OUTPUT);
  pinMode(LED_TURN_RIGHT, OUTPUT);

}

/*
   Helper function to assist with LED output, it takes four parameters:
   
   * msg: Message to prefix on each line
   * val: Value to output
   * portNegative: Arduino port to set to low if the value is noticeably negative
   * portPositive: Arduino port to set to high if the value is noticeable positive
 */
void LEDOutput(char *msg, int val, int portNegative, int portPositive) {
#ifdef DEBUG
  Serial.print(msg);
#endif

  if(val < -10) {
    digitalWrite(portPositive, LOW);    
    digitalWrite(portNegative, HIGH);
#ifdef DEBUG
    Serial.println("Lighting Negative");
#endif
  } else if (val > 10) {
    digitalWrite(portNegative, LOW);    
    digitalWrite(portPositive, HIGH);
#ifdef DEBUG
    Serial.println("Lighting Positive");
#endif
  } else {
    digitalWrite(portNegative, LOW);    
    digitalWrite(portPositive, LOW);
#ifdef DEBUG
    Serial.println("Lighting OFF");
#endif
  }
}

/*
  Helper function to move the vehicle - or in the current incarnation lit up a set of LEDs instead
*/

void vehicleMove(int axis, int val) {
  switch(axis) {
    case AXIS_X:
      LEDOutput("X: ", val, LED_LEFT, LED_RIGHT);
      break;

    case AXIS_Y:
      LEDOutput("Y: ", val, LED_BACKWARDS, LED_FORWARD);
      break;
  }
}

/*
  Helper function to rotate the vehicle - or in the current incarnation lit up a set of LEDs instead
*/

void vehicleRotate(int val) {
  LEDOutput("R: ", val, LED_TURN_LEFT, LED_TURN_RIGHT);
  
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
  
  // If we are receiving control commands from the RC controller bypass autonomous mode

  if(channelHorizontal || channelVertical || channelRotate) {
    
    // Map all readings to a -100 .. +100 range so it is easy to deal with

    int directionHorizontal = map(channelHorizontal, PWM_LOW, PWM_HIGH, -100, 100);
    int directionVertical = map(channelVertical, PWM_LOW, PWM_HIGH, -100, 100);
    int angleRotate = map(channelRotate, PWM_LOW, PWM_HIGH, -100, 100);

#ifdef DEBUG    
    Serial.print("Manual control ");

    Serial.print("H: ");
    Serial.print(directionHorizontal);

    Serial.print(" V: ");
    Serial.print(directionVertical);

    Serial.print(" R: ");
    Serial.println(angleRotate);
#endif

    if(directionHorizontal) {
      vehicleMove(AXIS_X, directionHorizontal);
    }

    if(directionVertical) {
      vehicleMove(AXIS_Y, directionVertical);
    }
    
    if(angleRotate) {
      vehicleRotate(angleRotate);
    }
    
  } else {
    Serial.println("Autonomous control");
  }

#ifdef DEBUG  
  delay(500);
#endif
}
