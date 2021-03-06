
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

const String longhornStr = "(150,72)(146,67)(146,67)(123,63)(123,63)(107,54)(107,54)(58,22)(58,22)(59,21)(59,21)(72,15)(72,15)(71,8)(71,8)(55,3)(55,3)(37,8)(37,8)(27,-19)(27,-19)(21,-40)(21,-40)(22,-50)(22,-50)(23,-64)(23,-64)(10,-76)(10,-76)(-8,-76)(-8,-76)(-9,-75)(-9,-75)(-21,-58)(-21,-58)(-17,-38)(-17,-38)(-23,-20)(-23,-20)(-31,0)(-31,0)(-32,8)(-32,8)(-45,4)(-45,4)(-60,5)(-60,5)(-68,14)(-68,14)(-68,15)(-68,15)(-53,22)(-53,22)(-120,63)(-120,63)(-145,66)(-145,66)(-150,68)(-150,68)(-143,75)(-143,75)(-134,75)(-134,75)(-134,75)(-134,75)(-47,38)(-47,38)(-33,38)(-33,38)(-22,42)(-22,42)(-12,41)(-12,41)(-3,42)(-3,42)(6,42)(6,42)(17,42)(17,42)(30,40)(30,40)(45,37)(45,37)(89,57)(89,57)(115,71)(115,71)(115,71)(115,71)(138,76)(138,76)";
const String longhornStrLong = "(150,72)(150,71)(150,70)(150,69)(149,69)(149,68)(149,67)(148,67)(147,67)(147,66)(146,66)(145,66)(144,66)(143,66)(142,66)(141,66)(140,66)(139,66)(138,66)(137,66)(136,66)(135,66)(134,66)(133,66)(133,65)(132,65)(131,65)(130,65)(129,65)(128,65)(128,64)(127,64)(126,64)(125,64)(124,64)(124,63)(123,63)(122,63)(121,63)(121,62)(120,62)(119,62)(119,61)(118,61)(117,61)(117,60)(116,60)(115,60)(115,59)(114,59)(114,58)(113,58)(112,58)(112,57)(111,57)(111,56)(110,56)(110,55)(109,55)(108,55)(108,54)(107,54)(107,53)(106,53)(106,52)(105,52)(104,52)(104,51)(103,51)(103,50)(102,50)(101,50)(101,49)(100,49)(100,48)(99,48)(98,48)(98,47)(97,47)(97,46)(96,46)(95,46)(95,45)(94,45)(94,44)(93,44)(92,44)(92,43)(91,43)(91,42)(90,42)(89,42)(89,41)(88,41)(87,41)(87,40)(86,40)(86,39)(85,39)(84,39)(84,38)(83,38)(83,37)(82,37)(81,37)(81,36)(80,36)(79,36)(79,35)(78,35)(78,34)(77,34)(76,34)(76,33)(75,33)(74,33)(74,32)(73,32)(73,31)(72,31)(71,31)(71,30)(70,30)(70,29)(69,29)(68,29)(68,28)(67,28)(66,28)(66,27)(65,27)(65,26)(64,26)(63,26)(63,25)(62,25)(61,24)(60,24)(60,23)(59,23)(58,23)(58,22)(58,21)(59,21)(59,20)(60,20)(61,20)(62,20)(62,19)(63,19)(64,19)(64,18)(65,18)(66,18)(66,17)(67,17)(68,17)(68,16)(69,16)(70,16)(71,16)(71,15)(72,15)(73,15)(73,14)(73,13)(74,13)(74,12)(74,11)(73,11)(73,10)(73,9)(72,9)(72,8)(71,8)(71,7)(70,7)(69,7)(69,6)(68,6)(67,6)(67,5)(66,5)(65,5)(64,5)(64,4)(63,4)(62,4)(61,4)(61,3)(60,3)(59,3)(58,3)(57,3)(56,3)(55,3)(54,3)(53,3)(53,4)(52,4)(51,4)(50,4)(49,4)(49,5)(48,5)(47,5)(46,5)(46,6)(45,6)(44,6)(43,6)(43,7)(42,7)(41,7)(41,8)(40,8)(39,8)(38,8)(37,8)(37,7)(37,6)(37,5)(37,4)(37,3)(37,2)(36,2)(36,1)(36,0)(36,-1)(36,-2)(36,-3)(35,-3)(35,-4)(35,-5)(35,-6)(34,-6)(34,-7)(34,-8)(34,-9)(33,-9)(33,-10)(33,-11)(32,-11)(32,-12)(32,-13)(31,-13)(31,-14)(31,-15)(30,-15)(30,-16)(29,-16)(29,-17)(29,-18)(28,-18)(28,-19)(27,-19)(27,-20)(26,-20)(26,-21)(26,-22)(25,-22)(25,-23)(25,-24)(24,-24)(24,-25)(24,-26)(23,-26)(23,-27)(23,-28)(23,-29)(22,-29)(22,-30)(22,-31)(22,-32)(22,-33)(22,-34)(21,-34)(21,-35)(21,-36)(21,-37)(21,-38)(21,-39)(21,-40)(21,-41)(21,-42)(21,-43)(21,-44)(21,-45)(22,-45)(22,-46)(22,-47)(22,-48)(22,-49)(23,-49)(23,-50)(23,-51)(23,-52)(23,-53)(23,-54)(23,-55)(24,-55)(24,-56)(24,-57)(24,-58)(24,-59)(24,-60)(24,-61)(24,-62)(24,-63)(24,-64)(23,-64)(23,-65)(22,-65)(22,-66)(21,-66)(21,-67)(20,-68)(20,-69)(19,-69)(19,-70)(18,-70)(18,-71)(17,-71)(16,-71)(16,-72)(15,-72)(15,-73)(14,-73)(14,-74)(13,-74)(13,-75)(12,-75)(11,-75)(10,-75)(9,-75)(8,-75)(8,-76)(7,-76)(6,-76)(5,-76)(4,-76)(3,-76)(2,-76)(1,-76)(0,-76)(-1,-76)(-2,-76)(-3,-76)(-4,-76)(-5,-76)(-6,-76)(-6,-75)(-7,-75)(-8,-75)(-9,-75)(-9,-74)(-10,-74)(-11,-74)(-11,-73)(-12,-73)(-12,-72)(-13,-72)(-13,-71)(-14,-71)(-14,-70)(-15,-70)(-15,-69)(-16,-69)(-16,-68)(-17,-68)(-17,-67)(-18,-67)(-18,-66)(-19,-66)(-19,-65)(-19,-64)(-20,-64)(-20,-63)(-20,-62)(-20,-61)(-20,-60)(-20,-59)(-20,-58)(-20,-57)(-20,-56)(-20,-55)(-20,-54)(-20,-53)(-20,-52)(-19,-52)(-19,-51)(-19,-50)(-19,-49)(-18,-49)(-18,-48)(-18,-47)(-18,-46)(-18,-45)(-17,-45)(-17,-44)(-17,-43)(-17,-42)(-17,-41)(-17,-40)(-17,-39)(-17,-38)(-17,-37)(-17,-36)(-17,-35)(-17,-34)(-17,-33)(-18,-33)(-18,-32)(-18,-31)(-18,-30)(-18,-29)(-19,-29)(-19,-28)(-19,-27)(-19,-26)(-20,-26)(-20,-25)(-20,-24)(-21,-24)(-21,-23)(-21,-22)(-22,-22)(-22,-21)(-22,-20)(-22,-19)(-23,-19)(-23,-18)(-24,-18)(-24,-17)(-24,-16)(-25,-16)(-25,-15)(-25,-14)(-26,-14)(-26,-13)(-26,-12)(-27,-12)(-27,-11)(-27,-10)(-28,-10)(-28,-9)(-28,-8)(-29,-8)(-29,-7)(-29,-6)(-29,-5)(-30,-5)(-30,-4)(-30,-3)(-30,-2)(-30,-1)(-31,-1)(-31,0)(-31,1)(-31,2)(-31,3)(-31,4)(-31,5)(-31,6)(-31,7)(-32,7)(-32,8)(-33,8)(-34,8)(-35,8)(-36,8)(-37,8)(-37,7)(-38,7)(-39,7)(-39,6)(-40,6)(-41,6)(-41,5)(-42,5)(-43,5)(-43,4)(-44,4)(-45,4)(-46,4)(-46,3)(-47,3)(-48,3)(-49,3)(-50,3)(-51,3)(-52,3)(-52,4)(-53,4)(-54,4)(-55,4)(-56,4)(-57,4)(-58,4)(-58,5)(-59,5)(-60,5)(-61,5)(-61,6)(-62,6)(-63,6)(-63,7)(-64,7)(-65,7)(-65,8)(-66,8)(-67,8)(-67,9)(-68,9)(-68,10)(-69,10)(-69,11)(-69,12)(-69,13)(-68,13)(-68,14)(-68,15)(-67,15)(-66,15)(-66,16)(-65,16)(-64,16)(-64,17)(-63,17)(-62,17)(-62,18)(-61,18)(-60,18)(-60,19)(-59,19)(-58,19)(-58,20)(-57,20)(-56,20)(-55,20)(-55,21)(-54,21)(-54,22)(-53,22)(-54,22)(-54,23)(-55,23)(-56,23)(-56,24)(-57,24)(-58,24)(-58,25)(-59,25)(-60,25)(-60,26)(-61,26)(-62,27)(-63,27)(-63,28)(-64,28)(-65,28)(-65,29)(-66,29)(-67,29)(-67,30)(-68,30)(-69,30)(-69,31)(-70,31)(-70,32)(-71,32)(-72,32)(-72,33)(-73,33)(-74,33)(-74,34)(-75,34)(-76,35)(-77,35)(-77,36)(-78,36)(-79,36)(-79,37)(-80,37)(-81,37)(-81,38)(-82,38)(-82,39)(-83,39)(-84,39)(-84,40)(-85,40)(-85,41)(-86,41)(-87,41)(-87,42)(-88,42)(-89,43)(-90,43)(-90,44)(-91,44)(-92,44)(-92,45)(-93,45)(-93,46)(-94,46)(-95,46)(-95,47)(-96,47)(-96,48)(-97,48)(-98,48)(-98,49)(-99,49)(-99,50)(-100,50)(-101,50)(-101,51)(-102,51)(-102,52)(-103,52)(-104,52)(-104,53)(-105,53)(-105,54)(-106,54)(-107,54)(-107,55)(-108,55)(-108,56)(-109,56)(-110,56)(-110,57)(-111,57)(-111,58)(-112,58)(-113,58)(-113,59)(-114,59)(-114,60)(-115,60)(-116,60)(-116,61)(-117,61)(-117,62)(-118,62)(-119,62)(-119,63)(-120,63)(-121,63)(-122,63)(-123,63)(-123,64)(-124,64)(-125,64)(-126,64)(-127,64)(-127,65)(-128,65)(-129,65)(-130,65)(-131,65)(-132,65)(-133,65)(-133,66)(-134,66)(-135,66)(-136,66)(-137,66)(-138,66)(-139,66)(-140,66)(-141,66)(-142,66)(-143,66)(-144,66)(-145,66)(-146,66)(-147,66)(-147,67)(-148,67)(-149,67)(-149,68)(-150,68)(-150,69)(-150,70)(-150,71)(-150,72)(-149,72)(-149,73)(-148,73)(-148,74)(-147,74)(-146,74)(-146,75)(-145,75)(-144,75)(-143,75)(-142,75)(-138,76)(-133,75)(-132,75)(-131,75)(-130,75)(-129,75)(-128,75)(-127,75)(-126,75)(-126,74)(-125,74)(-124,74)(-123,74)(-122,74)(-121,74)(-121,73)(-120,73)(-119,73)(-118,73)(-118,72)(-117,72)(-116,72)(-115,72)(-115,71)(-114,71)(-113,71)(-113,70)(-112,70)(-111,70)(-111,69)(-110,69)(-109,69)(-108,68)(-107,68)(-106,68)(-106,67)(-105,67)(-104,67)(-104,66)(-103,66)(-102,65)(-101,65)(-101,64)(-100,64)(-99,64)(-99,63)(-98,63)(-97,63)(-97,62)(-96,62)(-95,62)(-95,61)(-94,61)(-94,60)(-93,60)(-92,60)(-92,59)(-91,59)(-90,59)(-90,58)(-89,58)(-88,58)(-88,57)(-87,57)(-87,56)(-86,56)(-85,56)(-85,55)(-84,55)(-83,55)(-83,54)(-82,54)(-81,54)(-81,53)(-80,53)(-80,52)(-79,52)(-78,52)(-78,51)(-77,51)(-76,51)(-76,50)(-75,50)(-74,50)(-74,49)(-73,49)(-72,49)(-72,48)(-71,48)(-70,48)(-70,47)(-69,47)(-68,47)(-68,46)(-67,46)(-66,46)(-66,45)(-65,45)(-64,45)(-64,44)(-63,44)(-62,44)(-62,43)(-61,43)(-60,43)(-59,43)(-59,42)(-58,42)(-57,42)(-57,41)(-56,41)(-55,41)(-54,41)(-54,40)(-53,40)(-52,40)(-51,40)(-51,39)(-50,39)(-49,39)(-48,39)(-48,38)(-47,38)(-46,38)(-45,38)(-44,38)(-44,37)(-43,37)(-42,37)(-41,37)(-40,37)(-39,37)(-38,37)(-37,37)(-36,37)(-35,37)(-34,37)(-34,38)(-33,38)(-32,38)(-31,38)(-31,39)(-30,39)(-29,39)(-28,39)(-28,40)(-27,40)(-26,40)(-25,40)(-25,41)(-24,41)(-23,41)(-22,41)(-22,42)(-21,42)(-20,42)(-19,42)(-18,42)(-17,42)(-16,42)(-15,42)(-14,42)(-13,42)(-13,41)(-12,41)(-11,41)(-10,41)(-9,41)(-8,41)(-7,41)(-6,41)(-6,42)(-5,42)(-4,42)(-3,42)(-2,42)(-2,41)(-1,41)(0,41)(1,41)(2,41)(3,41)(4,41)(5,41)(5,42)(6,42)(7,42)(8,42)(9,42)(10,42)(10,41)(11,41)(12,41)(13,41)(14,41)(15,41)(16,41)(16,42)(17,42)(18,42)(19,42)(20,42)(21,42)(22,42)(23,42)(24,42)(25,42)(25,41)(26,41)(27,41)(28,41)(28,40)(29,40)(30,40)(31,40)(32,40)(32,39)(33,39)(34,39)(35,39)(35,38)(36,38)(37,38)(38,38)(38,37)(39,37)(40,37)(41,37)(42,37)(43,37)(44,37)(45,37)(46,37)(47,37)(47,38)(48,38)(49,38)(50,38)(51,38)(51,39)(52,39)(53,39)(54,39)(54,40)(55,40)(56,40)(57,40)(57,41)(58,41)(59,41)(60,41)(60,42)(61,42)(62,42)(62,43)(63,43)(64,43)(64,44)(65,44)(66,44)(67,44)(67,45)(68,45)(69,45)(69,46)(70,46)(71,46)(71,47)(72,47)(73,47)(73,48)(74,48)(75,48)(75,49)(76,49)(77,49)(77,50)(78,50)(78,51)(79,51)(80,51)(80,52)(81,52)(82,52)(82,53)(83,53)(84,53)(84,54)(85,54)(85,55)(86,55)(87,55)(87,56)(88,56)(88,57)(89,57)(90,57)(90,58)(91,58)(92,58)(92,59)(93,59)(94,59)(94,60)(95,60)(95,61)(96,61)(97,61)(97,62)(98,62)(98,63)(99,63)(100,63)(100,64)(101,64)(102,64)(102,65)(103,65)(103,66)(104,66)(105,66)(105,67)(106,67)(107,67)(107,68)(108,68)(109,68)(109,69)(110,69)(111,69)(111,70)(112,70)(113,70)(113,71)(114,71)(115,71)(115,72)(116,72)(117,72)(118,72)(118,73)(119,73)(120,73)(121,73)(121,74)(122,74)(123,74)(124,74)(125,74)(126,74)(126,75)(127,75)(128,75)(129,75)(130,75)(131,75)(132,75)(133,75)(134,75)(135,75)(135,76)(136,76)(137,76)(138,76)(139,76)(140,76)(141,76)(141,75)(142,75)(143,75)(144,75)(145,75)(146,75)(146,74)(147,74)(148,74)(148,73)(149,73)(149,72)(150,72)";

#include <Wire.h>
#include <AccelStepper.h>
//#include <Adafruit_MotorShield.h>
#include <ESP8266WiFi.h>



const char* ssid = "Cole";
const char* password = "cole1234";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
#define WIFI_TIMEOUT 2    //seconds

#define ARR_LENGTH 150

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
  pos points[ARR_LENGTH];
  int points_arr_size = 0;
  int points_arr_index = 0;
};


/* Polargraph status */
pos pos_current = {0, 0};     //current position of our drawing instrument
line line_current;      //current shape being drawn for drawLine
polygon poly_current;   //current shape being drawn for drawPolygon
polygon longhorn;

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

  long timeoutTime = millis() + WIFI_TIMEOUT * 1000;
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

  longhorn = parsePolygonString(longhornStrLong);

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
    Serial.println("Connected " + String(poly_current.points_arr_index) + "/" + String(poly_current.points_arr_size) + " points.");
    if (!serialMode) {
      //wifi notification
    }
  }
}

bool checkCmds(String input) {
  if (input.equals("longhorn")) {
    setCurrentPolygon(longhorn);
  }
  else if (input.charAt(0) != '(') {
    Serial.println("Error: invalid input");
    return false;
  }
  else {
    return false;
  }
  return true;

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

    if (checkCmds(input)) {
      return true;
    }
    
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
  int numPoints = 0;

  int strIndex = 0;
  while (((strIndex = polyStr.indexOf('(', strIndex)) != -1) && (numPoints < ARR_LENGTH)) {
    strIndex++;
    int commaIndex = polyStr.indexOf(',', strIndex);
    int endIndex = polyStr.indexOf(')', strIndex);
    int x = polyStr.substring(strIndex, commaIndex).toInt();
    int y = polyStr.substring(commaIndex + 1, endIndex).toInt();
    poly.points[numPoints] = {x, y};
    numPoints++;
  }
  poly.points_arr_size = numPoints;
  poly.points_arr_index = 0;
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
    //Serial.println(poly_current.points[poly_current.points_arr_index].x);
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

    Serial.println("Setting to Pos: x=" + String(pos_new.x) + " y=" + String(pos_new.y));
    Serial.println("Diff Lengths: L=" + String(left_length_new - left_length) + " R=" + String(right_length_new - right_length));
    Serial.println("Old Lengths: L=" + String(left_length) + " R=" + String(right_length));
    Serial.println("New Lengths: L=" + String(left_length_new) + " R=" + String(right_length_new));
    Serial.println("Steps: L=" + String(left_steps) + " R=" + String(right_steps) + "\n");

    if ((left_steps != 0) || (right_steps != 0)) {
      left_length = left_length_new;
      right_length = right_length_new;
      pos_current = pos_new;

      //this starts the movement, each motor is incremented a small amount each loop, very quickly
      stepperL.move(left_steps);
      stepperR.move(right_steps);
    }
    
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

