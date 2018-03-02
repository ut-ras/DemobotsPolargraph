/* DEMOBOTS POLARGRAPH :) 2018
 * 
 */
 
#include <Wire.h>
#include <AccelStepper.h>
//#include <Adafruit_MotorShield.h>

/*
 * Install AccelStepper Library (Sketch->Tools->Manage Libraries)
 *    Cool library for interleaving steppers, easy to use with different motor drivers
 *
 * Width: 48in (motors are 48in apart) = 1220mm
 * Height: 30in = 762  mm
 * 
 * Center of board is 0, 0
 * X range [-610, 610] mm
 * Y range [-380, 380] mm
 * 
 */

#define X_MAX 610
#define Y_MAX 380



//unit of measurement: mm
typedef struct pos_t {
  int x = 0;
  int y = 0;  
} pos;

//current position of our drawing instrument
pos pos_current;




//H-Bridge AccelStepper objects
AccelStepper stepperL(4, 0, 1, 2, 3);
AccelStepper stepperR(4, 4, 5, 6, 7);



void setup() {
    Serial.begin(9600);
    Serial.println("setup starting");
  
    //AFMS.begin();     //for Adafruit Motor Shield (see below)

    stepperL.setMaxSpeed(200.0);
    stepperL.setAcceleration(100.0);
    stepperL.moveTo(1000);
      
    stepperR.setMaxSpeed(200.0);
    stepperR.setAcceleration(100.0);
    stepperR.moveTo(1000);
    
    Serial.println("setup finished");
    
}

void loop() {
  testAccelStepperLib();

  
  
  stepperL.run();
  stepperR.run();
}









/* Coordinate Functions for pos struct */

//check if an x coordinate will fit in our system
bool isValidX(int x) {
  return (x > X_MAX*-1) && (x < X_MAX);
}

//check if a y coordinate will fit in our system
bool isValidY(int y) {
  return (y > Y_MAX*-1) && (y < Y_MAX);
}

//distance between coordinates p1 and p2
int getDistance(pos p1, pos p2) {
  return sqrt(exp(fabs(p2.x - p1.x)) + exp(fabs(p2.y - p1.y)));
}


/* Helper Functions */

//this lets us use == with pos struct
bool operator==(const pos& lhs, const pos& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}



/* Test for AccelStepper Library */
void testAccelStepperLib() {
  if (stepperL.distanceToGo() == 0) {
    stepperL.moveTo(-stepperL.currentPosition());
  }
  if (stepperR.distanceToGo() == 0) {
    stepperR.moveTo(-stepperR.currentPosition());
  }
}





/* //Adafruit Motor Shield v2 initialization - move to top for AFMS
   //Not using AFMS until we get a new one, board is broken, short across m3m4
   //We can easily replace the H-Bridge AccelStepper objects once we fix motor shield
   
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
*/

/* Install Motor Shield Library (Sketch->Tools->Manage Libraries)
 * https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/install-software
 * 
 * Testing a stepper: File>Examples>Adafruit Motor Shield V2 > StepperTest
 * AF Stepper Motor reference https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-stepper-motors
 */

