//DEMOBOTS POLARGRAPH :)

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

/* Install Motor Shield Library (Sketch->Tools->Manage Libraries)
 * https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/install-software
 * Stepper Motor reference
 * https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-stepper-motors
 */

/* By default, the motor will 'hold' the position after its done stepping
 * If you want to release all the coils, so that it can spin freely, call release()
 */
 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *left = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *right = AFMS.getStepper(200, 2);
    
void setup() {
    Serial.begin(9600);
    Serial.println("setup starting");
  
    AFMS.begin();
    
    right->setSpeed(10);
    //right->release();
    
    Serial.println("setup finished");
    
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Single coil steps");
  right->step(100, FORWARD, SINGLE); 
  right->step(100, BACKWARD, SINGLE); 

  Serial.println("Double coil steps");
  right->step(100, FORWARD, DOUBLE); 
  right->step(100, BACKWARD, DOUBLE);
  
  Serial.println("Interleave coil steps");
  right->step(100, FORWARD, INTERLEAVE); 
  right->step(100, BACKWARD, INTERLEAVE); 
  
  Serial.println("Microstep steps");
  right->step(50, FORWARD, MICROSTEP); 
  right->step(50, BACKWARD, MICROSTEP);
  
  delay(1000);
}
