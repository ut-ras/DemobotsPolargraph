
/* DEMOBOTS POLARGRAPH :) 2018 Spring


   Install AccelStepper Library (Sketch->Tools->Manage Libraries)
      Cool library for interleaving steppers, easy to use with different motor drivers

   Measurements are in mm for all dimensions

   Width: 48in (motors are 48in apart) = 1220mm
   Height: 30in = 762  mm

   Center of board is 0, 0
   X range [-610, 610] mm
   Y range [-380, 380] mm

   NEMA 17 stepper motors, 200 steps
   Left Motor: clockwise=down, countercw=up
   Right Motor: countercw=down, clockwise=up
   steps = dist * (STEPS_PER_ROT / PULLEY_CIRC)

   drawing instrument should ideally start and end center of board when power on/off


   TODO
   send http response after finishes current polygon (like the serial 'k')
   picking up and placing marker with small servo
   more tests for demonstration


   test serial input
   (0,0)(100,0)(100,100)(0,100)(0,0)
*/

String longhorn = "(150,72)(146,67)(146,67)(123,63)(123,63)(107,54)(107,54)(58,22)(58,22)(59,21)(59,21)(72,15)(72,15)(71,8)(71,8)(55,3)(55,3)(37,8)(37,8)(27,-19)(27,-19)(21,-40)(21,-40)(22,-50)(22,-50)(23,-64)(23,-64)(10,-76)(10,-76)(-8,-76)(-8,-76)(-9,-75)(-9,-75)(-21,-58)(-21,-58)(-17,-38)(-17,-38)(-23,-20)(-23,-20)(-31,0)(-31,0)(-32,8)(-32,8)(-45,4)(-45,4)(-60,5)(-60,5)(-68,14)(-68,14)(-68,15)(-68,15)(-53,22)(-53,22)(-120,63)(-120,63)(-145,66)(-145,66)(-150,68)(-150,68)(-143,75)(-143,75)(-134,75)(-134,75)(-134,75)(-134,75)(-47,38)(-47,38)(-33,38)(-33,38)(-22,42)(-22,42)(-12,41)(-12,41)(-3,42)(-3,42)(6,42)(6,42)(17,42)(17,42)(30,40)(30,40)(45,37)(45,37)(89,57)(89,57)(115,71)(115,71)(115,71)(115,71)(138,76)(138,76)(150,72)";

#include <Wire.h>
#include <AccelStepper.h>
//#include <Adafruit_MotorShield.h>
#include <ESP8266WiFi.h>



const char* ssid = "Cole";
const char* password = "cole1234";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


/* Measurements (mm)*/
#define X_MAX 560
#define Y_MAX 330
#define PULLEY_RADIUS 40  //update this
#define PULLEY_CIRC (2 * PULLEY_RADIUS * PI)
#define STEPS_PER_ROT 200
//steps = distance * mm_to_steps_pulley
double mm_to_steps_pulley = double(STEPS_PER_ROT) / double(PULLEY_CIRC);

bool serialMode = false;

/* Structs */

struct pos {
  int x;
  int y;
  pos& operator=(const pos& a) {
    x = a.x;
    y = a.y;
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
  pos points[30];
  int points_arr_size;
  int points_arr_index = 0;
};


/* Polargraph status */
pos pos_current = {0, 0};     //current position of our drawing instrument
line line_current;      //current shape being drawn for drawLine
polygon poly_current;   //current shape being drawn for drawPolygon

boolean isDrawing = false;

//keep track of current string length, less computation
int left_length;
int right_length;

//H-Bridge AccelStepper objects
//AccelStepper stepperL(4, 0, 1, 2, 3);
//AccelStepper stepperR(4, 4, 5, 6, 7);

//Stepper Driver AccelStepper objects (1, pinStep, pinDirection);
//pin in bottom left corner of drive is the dir pin
AccelStepper stepperR(1, 12, 13);                   //rstep = gpio12, rdir = gpio13
AccelStepper stepperL(1, 4, 5);                     //lstep = gpio4, ldir = gpio5


void connectWiFi() {
  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  long timeoutTime = millis() + 10000;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > timeoutTime) {
      Serial.println("\nCouldn't connect to WiFi, entering Serial mode.");
      serialMode = true;
      break;
    }
  }
  Serial.println("");

  if (!serialMode) {
    Serial.println("WiFi connected");
  
    // Start the server
    server.begin();
    Serial.println("Server started");
  
    // Print the IP address
    Serial.println(WiFi.localIP());
  }
}

/* Main */

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Welcome to Polargraph");

  connectWiFi();
  
  //only uncomment one setup function at a time
  setupPolargraph();
  //setupAccelStepperTest();      //this is only for AccelStepper test, all polargraph tests use setupPolargraph
  //setupTestShapes();          // add this for line/poly test
  //stepperTestRotateOnce();

  Serial.println("Welcome to the Demobots Polargraph! Send points over WiFi or Serial in this format: (x1,y1)(x2,y2)....");
}

void setupPolargraph() {
  stepperL.setMaxSpeed(200.0);
  stepperL.setAcceleration(100.0);
  stepperL.moveTo(0);

  stepperR.setMaxSpeed(200.0);
  stepperR.setAcceleration(100.0);
  stepperR.moveTo(0);


  left_length = getLeftStringLength(pos_current);
  right_length = getRightStringLength(pos_current);

  //polygon p = parsePolygonString(longhorn);
  //setCurrentPolygon(p);
}

void loop() {
  //note: extra lines will be drawn until we can pick up / place marker, i put TODO where we need that

  //runTests();

  checkSerial();
  checkWiFi();

  checkLineFinished();


  //increment the motors towards their goal
  stepperL.run();
  stepperR.run();
}


bool checkLineFinished() {
    if (isDrawing && drawPolygon()) {
    //send notification that the current line has finished
    Serial.println("k");
    if (!serialMode) {
      //wifi notification
    }
  }
}

bool checkWiFi() {
  if (serialMode) {
    return false;
  }
  
  WiFiClient client = server.available();
  if (!client) {
    return false;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  Serial.println(req);

  polygon p = parsePolygonString(req);
  setCurrentPolygon(p);

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\revieved coordinates</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  return true;
}

/* Serial Functions */

//read string from Serial to set position
//it will interrupt current drawing if you send over serial, we can add a check for finished if needed
//POLYGON FORMAT:   (x0,y0)(x1,y1)(x2,y2)(x3,y3)...
bool checkSerial() {
  if (Serial.available()) {
    String input = Serial.readString();
    polygon p = parsePolygonString(input);
    setCurrentPolygon(p);
    //Serial.println(input);  //for debugging, make sure this works
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
  poly.points_arr_size = numPoints;
  memcpy(poly.points, points, sizeof(points[0]) * poly.points_arr_size);
  return poly;
}








/* Polargraph Drawing Functions */

void setCurrentLine(line line_new) {
  line_current = line_new;
  isDrawing = true;
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
  isDrawing = true;
}

//use array to draw line at current index, set pos when it gets there and increment index. example in drawLine
bool drawPolygon() {
  if (poly_current.points_arr_index == 0 || checkIfNewPos()) {
    if (poly_current.points_arr_index == poly_current.points_arr_size) {
      //we have arrived at final destination
      isDrawing = false;
      return true;
    }
    setPos(poly_current.points[poly_current.points_arr_index]);
    poly_current.points_arr_index++;
  }
  return false;
}




/* Polargraph Positioning Functions */

/* Get the length of the string from the motor to the marker
*/
int getLeftStringLength(pos pos_new) {
  return sqrt(pow((X_MAX + pos_new.x), 2) + pow((Y_MAX - pos_new.y), 2)) ;
}

int getRightStringLength(pos pos_new) {
  return sqrt(pow((X_MAX - pos_new.x), 2) + pow((Y_MAX - pos_new.y), 2)) ;
}

/* Set position of drawing instrument, return false if invalid pos
   Position (0, 0) is center of the board
*/
bool setPos(pos pos_new) {
  if (isValidPos(pos_new)) {

    int left_length_new = getLeftStringLength(pos_new);
    int right_length_new = getRightStringLength(pos_new);

    //mm_to_steps_pulley is (steps per rotation) / (pulley circumference)
    int left_steps = (left_length_new - left_length) * mm_to_steps_pulley;
    int right_steps = -1 * (right_length_new - right_length) * mm_to_steps_pulley;

    //this starts the movement, each motor is incremented a small amount each loop, very quickly
    stepperL.move(left_steps);
    stepperR.move(right_steps);

    //Serial.println("Setting to Pos: x=" + String(pos_new.x) + " y=" + String(pos_new.y));
    //Serial.println("Diff Lengths: L=" + String(left_length_new - left_length) + " R=" + String(right_length_new - right_length));
    //Serial.println("Old Lengths: L=" + String(left_length) + " R=" + String(right_length));
    //Serial.println("New Lengths: L=" + String(left_length_new) + " R=" + String(right_length_new));
    //Serial.println("Steps: L=" + String(left_steps) + " R=" + String(right_steps) + "\n");

    left_length = left_length_new;
    right_length = right_length_new;
    pos_current = pos_new;
    return true;
  }
  else {
    return false;
  }
}

bool checkIfNewPos() {
  return isDrawing && (stepperL.distanceToGo() == 0) && (stepperR.distanceToGo() == 0);
}






/* Coordinate Functions for pos struct */

//check if an x coordinate will fit in our system
bool isValidX(int x) {
  return (x > X_MAX * -1) && (x < X_MAX);
}

//check if a y coordinate will fit in our system
bool isValidY(int y) {
  return (y > Y_MAX * -1) && (y < Y_MAX);
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

//make sure to setCurrentPolygon in setup
void drawPolyTest() {
  if (drawPolygon()) {
    poly_current.points_arr_index = 0;
  }
  drawPolygon();
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

void runTests() {
  //only uncomment one test at a time
  //testAccelStepperLib();
  //setPosTest();
  //drawLineTest();
  drawPolyTest();
}

void setupTestShapes() {
  pos p1 = {0, 0};
  pos p2 = {100, 100};
  line l = {p1, p2};
  setCurrentLine(l);

  polygon p;
  pos points[5] = {{0, 0}, {0, 300}, {300, 300}, {300, 0}, {0, 0}};
  p.points_arr_size = 5;
  memcpy(p.points, points, sizeof(points[0]) * p.points_arr_size);
  setCurrentPolygon(p);
}

void stepperTestRotateOnce() {
  stepperL.move(200);
  stepperR.move(200);
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
   https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/install-software

   Testing a stepper: File>Examples>Adafruit Motor Shield V2 > StepperTest
   AF Stepper Motor reference https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-stepper-motors
*/

