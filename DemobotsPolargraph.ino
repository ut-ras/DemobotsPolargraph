#include <Adafruit_MotorShield.h>

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
Adafruit_StepperMotor *left;
Adafruit_StepperMotor *right;
    
void setup() {
    Serial.begin(9600);
    Serial.println("setup starting");

    left = AFMS.getStepper(200, 1);
    right = AFMS.getStepper(200, 2);
  
    AFMS.begin();
    
    right->release();
    Serial.print("setup finished");
    
}

void loop() {
  // put your main code here, to run repeatedly:
  right->step(200, FORWARD, SINGLE);
  delay(100);
}
