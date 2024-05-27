//libs
#include <tcs3200.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

String lastTurn = "";
char lastColor;

//bools
bool blacklineDetected = false;
bool isTurningRight = false;
bool isTurningLeft = false;
bool isTurningAround = false;
bool isMovingTurn = false;
bool adjustTurn = false;
bool beginTurnAroundFinish = false;
bool finishedCourse = false;
bool skipTurn = false;

//timings
unsigned long irMillis = 0;
unsigned long motorMillis = 0;
unsigned long ultrasonicMillis = 0;
unsigned long turnTime = 0;
unsigned long moveTime = 0;
unsigned long firstDetectionTime1 = 0;
unsigned long firstDetectionTime4 = 0;

//constant
unsigned long sensorCheckThreshold = 50;

//motor driver pins
#define in1 5
#define in2 3
#define in3 9
#define in4 6

//colour sensor pins
#define out A5
#define S0 A4
#define S1 A1
#define S2 A0
#define S3 A2
#define OE A3

//rgb onboard
const int redLED = 26;
const int greenLED = 25;
const int blueLED = 27;

//ir sensor pins
const int ir1 = 4;
const int ir2 = 7;
const int ir3 = 11;
const int ir4 = 12;


//ultrasonic pins
const int trigPin = 8;
const int echoPin = 10;

//wifi info
char ssid[] = "MYHDSB";
char password[] = "";

void setup() {
  Serial.begin(250000);

  //onboard leds
  WiFiDrv::pinMode(redLED, OUTPUT);
  WiFiDrv::pinMode(greenLED, OUTPUT);
  WiFiDrv::pinMode(blueLED, OUTPUT);



  //connect to internet
  // while (!Serial)
  //   ;

  // Serial.println("attempting to connect");

  // while (WiFi.begin(ssid, password) != WL_CONNECTED) {
  //   WiFiDrv::analogWrite(redLED, 255);
  //   WiFiDrv::analogWrite(greenLED, 0);
  //   WiFiDrv::analogWrite(blueLED, 0);
  //   Serial.print(".");
  //   delay(5000);
  // }

  //once connected change led c olour
  Serial.println("CONNECTED!");
  WiFiDrv::analogWrite(redLED, 0);
  WiFiDrv::analogWrite(greenLED, 255);
  WiFiDrv::analogWrite(blueLED, 255);
  Serial.println(WiFi.localIP());

  //intializing colour sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(out, INPUT);

  //setting freq scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  //intitalizing ir sensors
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);

  //intializing ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //intalizing motors
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  colorSensor(); //running color sensor code, to check colours every cycle

  char currentColor = colorReading(); // make the current color into a var
  if (!(currentColor == lastColor)) { //checking if the currnet color is differnet tahn the previous
    if (currentColor == 'r') { //checking if the color is red
      // Serial.println("red");
      WiFiDrv::analogWrite(redLED, 255);
      WiFiDrv::analogWrite(greenLED, 0);
      WiFiDrv::analogWrite(blueLED, 0);
      finishedCourse = false; //red is beginning of course
    } else if (currentColor == 'b') { //checking if its blue
      // Serial.println("blue");
      WiFiDrv::analogWrite(redLED, 0);
      WiFiDrv::analogWrite(greenLED, 0);
      WiFiDrv::analogWrite(blueLED, 255);
    } else if (currentColor == 'y') { //checking if its yellow
      //  Serial.println("yellow");
      WiFiDrv::analogWrite(redLED, 255);
      WiFiDrv::analogWrite(greenLED, 255);
      WiFiDrv::analogWrite(blueLED, 0);
    } else if (currentColor == 'g') { //checking if its green
      WiFiDrv::analogWrite(redLED, 0);
      WiFiDrv::analogWrite(greenLED, 255);
      WiFiDrv::analogWrite(blueLED, 0);
      finishedCourse = true; //green means end of course, so we say we finished the course
    } else {
      Serial.println(colorReading()); //debug if all fails
    }
  }

  //millis
  unsigned long currentMillis = millis();
  int* valueArray = readInfrared();



  //turn around logic for wall detction
  if (currentMillis - ultrasonicMillis >= 20) {
    ultrasonicMillis = currentMillis;
    //Serial.println("time!");
    if (ultrasonicDist() <= 9 && isTurningAround == false) { //only check the dist if we arent in a turn around state, aka wall already detected
      // Serial.println("wall detected");
      isTurningAround = true;
      turnTime = currentMillis; //setting turn time to current time
    }
  }


  if (isTurningAround) { 
    if (currentMillis - turnTime >= 700) { //700 ms after initiaql turn around, we say we begin it
      beginTurnAroundFinish = true;
    }

    if (beginTurnAroundFinish) { 
      // Serial.println("finished intital turn");
      if (valueArray[1] == 1 || valueArray[2] == 1) { //when the turn aruond is "begun" check if its finished with two sensors

        isTurningAround = false;
        beginTurnAroundFinish = false;
        adjustableSpeed(-255, 255);
        delay(50);
        stop();
        delay(1000);
        // Serial.println("turn is done");
      } else { // if turn isnt finish, we continue turning
        adjustableSpeed(150, -150);
      }
    } else { //continue turning if we arent down the initial turn bit
      // Serial.println("not finished intital turn");
      adjustableSpeed(150, -150);
    }
  }


  //turn sequence
  if (isTurningAround == false) { //only run all of this if we arent in a turn around state

    if (isTurningLeft == false && isTurningRight == false && blacklineDetected == false) { //only run this if we arent in ANY kind of turn around state, (regular turn around)


      //checking which sensors are on and off, within a certain amount of time
      if (valueArray[0] == 1 && firstDetectionTime1 == 0) { 
        //Serial.print("sensor one detected ");
        //Serial.println(firstDetectionTime1);
        firstDetectionTime1 = currentMillis;
      } else if (valueArray[3] == 1 && firstDetectionTime4 == 0) {
        //Serial.print("sensor four detected ");
        //Serial.println(firstDetectionTime4);
        firstDetectionTime4 = currentMillis;
        // Serial.println(firstDetectionTime4);
      }


      //applying if we are on a black line or turning left or turning right, if our sensor has been detected, after a certain amount of time has passed
      if (firstDetectionTime1 > 0 && currentMillis - firstDetectionTime1 < sensorCheckThreshold && firstDetectionTime4 > 0 && currentMillis - firstDetectionTime4 < sensorCheckThreshold) {
        blacklineDetected = true; 
        isTurningLeft = false;
        isTurningRight = false;
        //resetting detection time to 0
        firstDetectionTime1 = 0; 
        firstDetectionTime4 = 0;
        //Serial.println("All detected");

      } else if (firstDetectionTime1 > 0 && currentMillis - firstDetectionTime1 >= sensorCheckThreshold) {
        blacklineDetected = false;
        isTurningLeft = false;
        isTurningRight = true;
        firstDetectionTime1 = 0;
        firstDetectionTime4 = 0;
        //Serial.println("turn left");
      } else if (firstDetectionTime4 > 0 && currentMillis - firstDetectionTime4 >= sensorCheckThreshold) {
        blacklineDetected = false;
        isTurningLeft = true;
        isTurningRight = false;
        firstDetectionTime1 = 0;
        firstDetectionTime4 = 0;
        //Serial.println("turn right");
      } else {
        //Serial.println("no logic");
      }
    }

    if (blacklineDetected == true) { //when we are on a black line
      if (lastTurn == "right") { //checking the last turn, depending what it is, we turn the same direction
        isTurningRight = true;
        isTurningLeft = false;
      } else if (lastTurn == "left") {
        isTurningLeft = true;
        isTurningRight = false;
      }
      blacklineDetected = false;
    }

    if (isTurningLeft == true && isTurningRight == false) { //checking if we are turning left
      isTurningLeft = true;
      lastTurn = "left"; //setting the last turn we did to the current  one

      if (!isMovingTurn) { //a timer made, so we can have a "bump" out
        adjustableSpeed(200, 200);
        delay(300);
        turnTime = currentMillis;
        isMovingTurn = true;
      }

      valueArray = readInfrared(); //checking the infared sensors again
      if (beginTurnAroundFinish == false && (valueArray[1] == 1 || valueArray[2] == 1)) { //if our beginning of our turn around isnt done, but our two sensors have been detected, we know we are on an intersectionj
        if (finishedCourse == true) { //if we finish the course, we skip this tourney
          skipTurn = true;
          if (skipTurn) { 
            isTurningLeft = false;
            isMovingTurn = false;
            beginTurnAroundFinish = false;
            skipTurn = false;
          }
        } else {
          if (currentMillis - turnTime >= 850) { //if we havent finished the course, then we have a slight amount of time for the turn to go
            beginTurnAroundFinish = true;
          }
        }
      } else if (skipTurn == false) { //if both is false, then we just assume its not on an intersection and ar egular turn, so ignore all logic
        beginTurnAroundFinish = true;
      }

      valueArray = readInfrared(); //getting the infared values
      if (beginTurnAroundFinish == true && isTurningLeft == true) { //checking if our beginning of turn around  has finished, and we are turning loeft
        if (valueArray[1] == 1 || valueArray[2] == 1) { //checking if sensors are detected, if so assume turn is done
          adjustableSpeed(255, -255); //reverse turn to adjust
          delay(40);
          stop();
          delay(250);
          isTurningLeft = false;
          beginTurnAroundFinish = false;
          isMovingTurn = false;
          skipTurn = false;
          //Serial.println("finished");
        } else if (isMovingTurn == true && isTurningLeft == true) { //if our turn isnt done, we keep turning
          adjustableSpeed(-150, 150);
          // Serial.println("finished intital");
        }
      } else if (isTurningLeft == true) { //if the intital isnt done we keep turning
        adjustableSpeed(-150, 150);
        //Serial.println("doing inital");
      }

    } else if (isTurningRight == true && isTurningLeft == false) { //exact same logic as turn left, but with adjustable speed being used to turn it right
      isTurningRight = true;
      lastTurn = "right";
      if (!isMovingTurn) {
        adjustableSpeed(200, 200);
        delay(300);
        turnTime = currentMillis;
        isMovingTurn = true;
      }

      valueArray = readInfrared();
      if (beginTurnAroundFinish == false && valueArray[1] == 1 || valueArray[2] == 1) {
        if (finishedCourse == true) {
          skipTurn = true;
          if (skipTurn) {
            isTurningRight = false;
            isMovingTurn = false;
            beginTurnAroundFinish = false;
            skipTurn = false;
          }
        } else {
          if (currentMillis - turnTime >= 850) {
            beginTurnAroundFinish = true;
          }
        }
      } else if (skipTurn == false) {
        beginTurnAroundFinish = true;
      }

      valueArray = readInfrared();
      if (beginTurnAroundFinish == true && isTurningRight == true) {
        if (valueArray[1] == 1 || valueArray[2] == 1) {
          adjustableSpeed(-255, 255);
          delay(50);
          stop();
          delay(250);
          isTurningRight = false;
          beginTurnAroundFinish = false;
          isMovingTurn = false;
          skipTurn = false;
        } else if (isMovingTurn == true && isTurningRight == true) {
          adjustableSpeed(150, -150);
        }
      } else if (isTurningRight == true) {
        adjustableSpeed(150, -150);
      }
    }

    if (isTurningLeft == false && isTurningRight == false) { //if we are just going on the black line trying to go straight, the logic to stay on it
      if (valueArray[0] == 0 && valueArray[1] == 1 && valueArray[2] == 1 && valueArray[3] == 0) { //if we both are on thel ine, just keep moving both motors same speed
        adjustableSpeed(200, 200);
      } else if (valueArray[0] == 0 && valueArray[1] == 1 && valueArray[2] == 0 && valueArray[3] == 0) { //if left on it and the other isnt, adjust to make sure they are both on it
        adjustableSpeed(200, 130);
        //Serial.println("adjust 1");
      } else if (valueArray[0] == 0 && valueArray[1] == 0 && valueArray[2] == 1 && valueArray[3] == 0) { //if right on it, and other isnt, adjust so both are
        adjustableSpeed(130, 200);
        //Serial.println("adjust 2");
      }
    }
  }

  lastColor = currentColor; //our last colour was our current color, will differenrate if a new color is fouind

  //debug
  // Serial.print(valueArray[0]);
  // Serial.print(" ");
  // Serial.print(valueArray[1]);
  // Serial.print(" ");
  // Serial.print(valueArray[2]);
  // Serial.print(" ");
  // Serial.print(valueArray[3]);
  // Serial.println(" ");
}
