
/* DEMOBOTS POLARGRAPH :) 2018 Spring
 * 
 * 
 * Install AccelStepper Library (Sketch->Tools->Manage Libraries)
 *    Cool library for interleaving steppers, easy to use with different motor drivers
 *
 * Measurements are in mm for all dimensions
 *
 * Width: 48in (motors are 48in apart) = 1220mm
 * Height: 30in = 762  mm
 * 
 * Center of board is 0, 0
 * X range [-610, 610] mm
 * Y range [-380, 380] mm
 * 
 * NEMA 17 stepper motors, 200 steps
 * Left Motor: clockwise=down, countercw=up
 * Right Motor: countercw=down, clockwise=up
 * steps = dist * (STEPS_PER_ROT / PULLEY_CIRC)
 * 
 * drawing instrument should ideally start and end center of board when power on/off
 * 
 * 
 * TODO
 * picking up and placing marker with small servo
 * draw polygon function
 * more tests for demonstration
 */

#include <Wire.h>
#include <AccelStepper.h>
//#include <Adafruit_MotorShield.h>



//Measurements (mm)
#define X_MAX 610
#define Y_MAX 380
#define PULLEY_RADIUS 80  //update this
#define PULLEY_CIRC (2 * PULLEY_RADIUS * PI)
#define STEPS_PER_ROT 200

//steps = distance * mm_to_steps_pulley
double mm_to_steps_pulley = double(STEPS_PER_ROT) / double(PULLEY_CIRC);

struct pos {
  int x;
  int y;

  pos& operator=(const pos& a) {
      x=a.x;
      y=a.y;
      return *this;
  }

  bool operator==(const pos& a) const {
      return (x == a.x && y == a.y);
  }
};

struct line {
  pos p1;
  pos p2;  
};

struct polygon {
  pos * points;
  int points_arr_size;
  int points_arr_index = 0;
};

//current position of our drawing instrument
pos pos_current = {0, 0};

//current shape being drawn for drawLine and drawPolygon
line line_current;
polygon poly_current;

//H-Bridge AccelStepper objects 
AccelStepper stepperL(4, 0, 1, 2, 3);
AccelStepper stepperR(4, 4, 5, 6, 7);


void setup() {
  //only uncomment one setup function at a time
  
  setupPolargraph();
  //setupAccelStepperTest();      //this is only for AccelStepper test, all polargraph tests use setupPolargraph
}

void setupPolargraph() {
  Serial.begin(9600);
  
  stepperL.setMaxSpeed(200.0);
  stepperL.setAcceleration(100.0);
  stepperL.moveTo(0);
  
  stepperR.setMaxSpeed(200.0);
  stepperR.setAcceleration(100.0);
  stepperR.moveTo(0);

  // add this for line test
  pos p1 = {0, 0};
  pos p2 = {100, 100};
  line l = {p1, p2};
  setCurrentLine(l);
}

void loop() {
  //note: extra lines will be drawn until we can pick up / place marker, i put TODO where we need that
  
  //only uncomment one test at a time
  //testAccelStepperLib();
  setPosTest();
  //drawLineTest();   

  
  //checkSerial();
  
  //increment the motors towards their goal
  stepperL.run();
  stepperR.run();
}

//read string from Serial to set position
//it will interrupt current drawing if you send over serial, we can add a check for finished if needed
//POLYGON FORMAT:   (x0,y0)(x1,y1)(x2,y2)(x3,y3)...
bool checkSerial() {
  if (Serial.available()) {
    String input = Serial.readString();
    polygon p = parsePolygonString(input);
    setCurrentPolygon(p);
    return true;
  }
  else {
    return false;
  }
}

polygon parsePolygonString(String polyStr) {
  polygon poly;
  pos points[100];
  int numPoints = 0;
  
  int strIndex = 0;
  while ((strIndex = polyStr.indexOf('(', strIndex)) != -1) {
    strIndex++;
    int commaIndex = polyStr.indexOf(',', strIndex);
    int endIndex = polyStr.indexOf(')', strIndex);
    int x = polyStr.substring(strIndex, commaIndex).toInt();
    int y = polyStr.substring(commaIndex + 1, endIndex).toInt();
    points[numPoints] = {x, y};
    numPoints++;
  }
  poly.points = points;
  poly.points_arr_size = numPoints;
  return poly;
}








/* Polargraph Drawing Functions */

void setCurrentLine(line line_new) {
  line_current = line_new;  
}

//draw line, call from loop, returns true when done, requires picking up/placing marker
bool drawLine() {
  if (pos_current == line_current.p2) {
    //have started drawing line
    if (checkIfNewPos()) {
      //TODO pick up marker
      return true;
    }
    return false;
  }
  else if (pos_current == line_current.p1) {
    //have started moving to the initial position
    if (checkIfNewPos()) {
      //TODO place marker
      setPos(line_current.p2);
    }
    return false;
  }
  else {
    //havent started moving to p1
    //TODO pick up marker
    setPos(line_current.p1);
    return false;
  }
}

void setCurrentPolygon(polygon poly_new) {
  poly_current = poly_new;
}

//use array to draw line at current index, set pos when it gets there and increment index. example in drawLine
bool drawPolygon() {
  if (poly_current.points_arr_index == 0 || checkIfNewPos()) {
    if (poly_current.points_arr_index == poly_current.points_arr_size) {
      //we have arrived at final destination
      return true;
    }
    setPos(poly_current.points[poly_current.points_arr_index]);
    poly_current.points_arr_index++;
  }
  return false;
}




/* Polargraph Positioning Functions */

int getLeftStringLength(pos pos_new) {
  return sqrt(pow((X_MAX + pos_new.x), 2) + pow((Y_MAX - pos_new.y), 2)) ; 
}

int getRightStringLength(pos pos_new) {
  return sqrt(pow((X_MAX - pos_new.x), 2) + pow((Y_MAX - pos_new.y), 2)) ; 
}

//Set position of polargraph drawing instrument, return false if invalid pos
bool setPos(pos pos_new) {
  if (isValidPos(pos_new)) {
    int left_length = getLeftStringLength(pos_current);
    int right_length = getRightStringLength(pos_current);

    int left_length_new= getLeftStringLength(pos_new);
    int right_length_new = getRightStringLength(pos_new);

    int left_steps = (left_length_new - left_length) * mm_to_steps_pulley;
    int right_steps = (right_length_new - right_length) * mm_to_steps_pulley;
    
    stepperL.move(left_steps);
    stepperR.move(right_steps);
    
    pos_current = pos_new;
    return true;
  }
  else {
    return false;
  }
}

bool checkIfNewPos() {
  return (stepperL.distanceToGo() == 0) && (stepperR.distanceToGo() == 0);
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

bool isValidPos(pos new_pos) {
  return isValidY(new_pos.y) && isValidX(new_pos.x);
}

//distance between coordinates p1 and p2
int getDistance(pos p1, pos p2) {
  return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}





/* Helper Functions */






/* Tests - meant to be called in a loop with run() calls at the end of each loop */

//should go back and forth between 0,0 and 100,100
void setPosTest() {
  if (checkIfNewPos()) {
    pos pos_new = {0, 0};
    if (pos_current.x == 0) {
      pos_new.x = 100;
      pos_new.y = 100;
    }
    setPos(pos_new);
  }
}

//make sure to setCurrentLine in setup. this test will draw the line once then do nothing
void drawLineTest() {
  static bool isFinished = false;
  //this should only call drawLine if it is not finished
  //      the static var allows us to call this in a loop but only draw the line once for a test
  
  if (!isFinished && drawLine()) {
    isFinished = true;
  }
  
}


//this is only for AccelStepper
void setupAccelStepperTest() {
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

//Test for AccelStepper Library
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

