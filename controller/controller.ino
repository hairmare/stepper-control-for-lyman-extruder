/**
 * code for controlling a lyman extruder
 *
 * uses these components:
 * - arduino nano
 *   http://arduino.cc/en/Main/ArduinoBoardNano
 * - reprap stepper motor driver v2.3 (SMDv2.3)
 *   http://reprap.org/wiki/Stepper_Motor_Driver_2.3
 *
 * Depends on these tools/libs
 * - Arduino IDE 1.0.1
 *   http://arduino.cc/en/Main/Software
 * - Potentiometer Library
 *   http://playground.arduino.cc/Code/Potentiometer 
 *
 * @author    Lucas Bickel <hairmare@purplehaze.ch>
 * @copyright 2013, Alle Rechte vorbehalten
 * @license   GNU Affero General Public License, version 3
 */
 
// setup fixed values
#define DEFAULT_DELAY 100 // gets added to value from potentiometer and influences the
                          // delay between each step

// setup pins
#define PIN_OUT_STEP        4 // pin 3 on the SMDv2.3
#define PIN_OUT_DIR         5 // pin 4 on the SMDv2.3
#define PIN_OUT_ENABLE      6 // pin 5 on the SMDv2.3
#define PIN_IN_POTI         2 // analog pin for potentiometer
#define PIN_IN_MOTOR_SWITCH 3 // hook this to GND through a switch and a small resistor
#define INT_MOTOR_SWITCH    0 // also define what interrupt the pin is on

// load libs
#include <Potentiometer.h>

// setup global state variables
volatile boolean runMotor = false;
Potentiometer potentiometer = Potentiometer(PIN_IN_POTI); 

/**
 * initialize pins
 * 
 * @return void
 */
void setup() {
  Serial.begin(9600);
  Serial.println("initializing lyman extruder");
  
  // pins for stepper driver
  pinMode(PIN_OUT_STEP, OUTPUT);
  pinMode(PIN_OUT_DIR, OUTPUT);
  pinMode(PIN_OUT_ENABLE, OUTPUT);
  
  // pin for motor switch
  pinMode(PIN_IN_MOTOR_SWITCH, INPUT);
  // turn on internal pull-up resistor
  digitalWrite(PIN_IN_MOTOR_SWITCH, HIGH);
  
  // disable stepper per default
  digitalWrite(PIN_OUT_ENABLE, HIGH);
  
  // set default direction
  digitalWrite(PIN_OUT_DIR, LOW); // @todo make this configurable by poti or button?
    
  // initialize interrupt for motor switch
  attachInterrupt(INT_MOTOR_SWITCH, startMotor, FALLING);
  attachInterrupt(INT_MOTOR_SWITCH, stopMotor, RISING);
    
  Serial.println("lyman extruder initialization complete, starting motor");
  startMotor();
}

/**
 * interrupt service routine for starting the motor
 * 
 * @return void
 */
void startMotor() {
  Serial.println("starting motor");
  runMotor = true;
  digitalWrite(PIN_OUT_ENABLE, LOW);
}

/**
 * interrupt service routine for stopping the motor
 * 
 * @return void
 */
void stopMotor() {
  Serial.println("stopping motor");
  runMotor = false;
  digitalWrite(PIN_OUT_ENABLE, HIGH);
}

/**
 * main program loop responsible for running the motor
 * 
 * @return void
 */
void loop() {
  if (runMotor) {
    int currentDelay = DEFAULT_DELAY + potentiometer.getValue();
    
    digitalWrite(PIN_OUT_STEP, HIGH);
    delayMicroseconds(currentDelay);
    digitalWrite(PIN_OUT_STEP, LOW);
    delayMicroseconds(currentDelay);
  }
}
