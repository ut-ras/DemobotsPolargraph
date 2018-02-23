//DEMOBOTS POLARGRAPH :)

#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

/* Install Motor Shield Library (Sketch->Tools->Manage Libraries)
 * https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/install-software
 * Install AccelStepper Library (Sketch->Tools->Manage Libraries)
 * 
 * Testing a stepper: File>Examples>Adafruit Motor Shield V2 > StepperTest
 * AF Stepper Motor reference https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-stepper-motors
 */

/* By default, the motor will 'hold' the position after its done stepping
 * If you want to release all the coils, so that it can spin freely, call release()
 */
 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *leftStepperAF = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *rightStepperAF = AFMS.getStepper(200, 2);

//wrappers for AccelStepper objects

void forwardstepL() {  
  leftStepperAF->onestep(FORWARD, SINGLE);
}
void backwardstepL() {  
  leftStepperAF->onestep(BACKWARD, SINGLE);
}

void forwardstepR() {  
  rightStepperAF->onestep(FORWARD, SINGLE);
}
void backwardstepR() {  
  rightStepperAF->onestep(BACKWARD, SINGLE);
}

AccelStepper stepperL(forwardstepL, backwardstepL);
AccelStepper stepperR(forwardstepR, backwardstepR);

void setup() {
    Serial.begin(9600);
    Serial.println("setup starting");
  
    AFMS.begin();

    stepperL.setMaxSpeed(200.0);
    stepperL.setAcceleration(100.0);
    stepperL.moveTo(5000);
      
    stepperR.setMaxSpeed(200.0);
    stepperR.setAcceleration(100.0);
    stepperR.moveTo(5000);
    
    Serial.println("setup finished");
    
}

void loop() {
  testAccelStepperLib();
  //testAFLib();
}


//test for AccelStepper library
void testAccelStepperLib() {
  if (stepperL.distanceToGo() == 0)
    stepperL.moveTo(-stepperL.currentPosition());

  if (stepperR.distanceToGo() == 0)
    stepperR.moveTo(-stepperR.currentPosition());
  
  stepperL.run();
  stepperR.run();
}




//test for default adafruit motor shield library
void testAFLib() {
  Serial.println("Single coil steps");
  rightStepperAF->step(100, FORWARD, SINGLE); 
  rightStepperAF->step(100, BACKWARD, SINGLE); 

  Serial.println("Double coil steps");
  rightStepperAF->step(100, FORWARD, DOUBLE); 
  rightStepperAF->step(100, BACKWARD, DOUBLE);
  
  Serial.println("Interleave coil steps");
  rightStepperAF->step(100, FORWARD, INTERLEAVE); 
  rightStepperAF->step(100, BACKWARD, INTERLEAVE); 
  
  Serial.println("Microstep steps");
  rightStepperAF->step(50, FORWARD, MICROSTEP); 
  rightStepperAF->step(50, BACKWARD, MICROSTEP);
}



