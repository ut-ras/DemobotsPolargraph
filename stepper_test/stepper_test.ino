#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"



void init(){
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();

    Adafruit_StepperMotor *left = AFMS.getStepper(200, 1);
    Adafruit_StepperMotor *right = AFMS.getStepper(200, 2);
    step(200, FORWARD, SINGLE);
}

void loop(){
    
}

