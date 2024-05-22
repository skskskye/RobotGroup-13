//libs
#include <tcs3200.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

String lastTurn = "";

//bools
bool blacklineDetected = false;
bool isTurningRight = false;
bool isTurningLeft = false;
bool isTurningAround = false;
bool isMovingTurn = false;
bool adjustTurn = false;
bool beginTurnAroundFinish = false;

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
  colorSensor();

  if (colorReading() == "red") {
    WiFiDrv::analogWrite(redLED, 255);
    WiFiDrv::analogWrite(greenLED, 0);
    WiFiDrv::analogWrite(blueLED, 0);
  }else if(colorReading() == "blue"){
    WiFiDrv::analogWrite(redLED, 0);
    WiFiDrv::analogWrite(greenLED, 0);
    WiFiDrv::analogWrite(blueLED, 255);
  }else if(colorReading() == "yellow"){
    WiFiDrv::analogWrite(redLED, 255);
    WiFiDrv::analogWrite(greenLED, 255);
    WiFiDrv::analogWrite(blueLED, 0);
  }



  //millis
  unsigned long currentMillis = millis();
  int* valueArray = readInfrared();

  //colorSensor();


  //turn around logic for wall detction
  if (currentMillis - ultrasonicMillis >= 20) {
    ultrasonicMillis = currentMillis;
    //Serial.println("time!");
    if (ultrasonicDist() <= 9 && isTurningAround == false) {
      // Serial.println("wall detected");
      isTurningAround = true;
      turnTime = currentMillis;
    }
  }
  if (isTurningAround) {
    if (currentMillis - turnTime >= 700) {
      beginTurnAroundFinish = true;
    }

    if (beginTurnAroundFinish) {
      // Serial.println("finished intital turn");
      if (valueArray[1] == 1 || valueArray[2] == 1) {

        isTurningAround = false;
        beginTurnAroundFinish = false;
        adjustableSpeed(-255, 255);
        delay(50);
        stop();
        delay(1000);
        // Serial.println("turn is done");
      } else {
        adjustableSpeed(150, -150);
      }
    } else {
      // Serial.println("not finished intital turn");
      adjustableSpeed(150, -150);
    }
  }


  //turn sequence
  if (isTurningAround == false) {

    if (isTurningLeft == false && isTurningRight == false && blacklineDetected == false) {

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


      if (firstDetectionTime1 > 0 && currentMillis - firstDetectionTime1 < sensorCheckThreshold && firstDetectionTime4 > 0 && currentMillis - firstDetectionTime4 < sensorCheckThreshold) {
        blacklineDetected = true;
        isTurningLeft = false;
        isTurningRight = false;
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


    if (blacklineDetected == true) {
      if (lastTurn == "right") {
        isTurningRight = true;
        isTurningLeft = false;
      } else if (lastTurn == "left") {
        isTurningLeft = true;
        isTurningRight = false;
      }
      blacklineDetected = false;
    }

    if (isTurningLeft == true && isTurningRight == false) {
      isTurningLeft = true;
      lastTurn = "left";



      if (!isMovingTurn) {
        adjustableSpeed(200, 200);
        delay(300);
        turnTime = currentMillis;
        isMovingTurn = true;
      }

      if (beginTurnAroundFinish == false && valueArray[1] == 1 || valueArray[2] == 1) {
        if (currentMillis - turnTime >= 850) {
          beginTurnAroundFinish = true;
        }
      } else {
        beginTurnAroundFinish = true;
      }



      valueArray = readInfrared();
      if (beginTurnAroundFinish) {
        if (valueArray[1] == 1 || valueArray[2] == 1) {
          adjustableSpeed(255, -255);
          delay(40);
          stop();
          delay(250);
          isMovingTurn = false;
          isTurningLeft = false;
          beginTurnAroundFinish = false;
          //Serial.println("finished");
        } else if (isMovingTurn) {
          adjustableSpeed(-150, 150);
          // Serial.println("finished intital");
        }
      } else {
        adjustableSpeed(-150, 150);
        //Serial.println("doing inital");
      }



    } else if (isTurningRight == true && isTurningLeft == false) {
      isTurningRight = true;
      lastTurn = "right";
      if (!isMovingTurn) {
        adjustableSpeed(200, 200);
        delay(300);
        turnTime = currentMillis;
        isMovingTurn = true;
      }

      if (beginTurnAroundFinish == false && valueArray[1] == 1 || valueArray[2] == 1) {
        if (currentMillis - turnTime >= 850) {
          beginTurnAroundFinish = true;
        }
      } else {
        beginTurnAroundFinish = true;
      }






      valueArray = readInfrared();
      if (beginTurnAroundFinish) {
        if (valueArray[1] == 1 || valueArray[2] == 1) {
          adjustableSpeed(-255, 255);
          delay(50);
          stop();
          delay(250);
          isTurningRight = false;
          beginTurnAroundFinish = false;
          isMovingTurn = false;
        } else if (isMovingTurn) {
          adjustableSpeed(150, -150);
        }
      } else {
        adjustableSpeed(150, -150);
      }
    }

    if (isTurningLeft == false && isTurningRight == false) {
      if (valueArray[0] == 0 && valueArray[1] == 1 && valueArray[2] == 1 && valueArray[3] == 0) {
        adjustableSpeed(200, 200);
      } else if (valueArray[0] == 0 && valueArray[1] == 1 && valueArray[2] == 0 && valueArray[3] == 0) {
        adjustableSpeed(200, 130);
        //Serial.println("adjust 1");
      } else if (valueArray[0] == 0 && valueArray[1] == 0 && valueArray[2] == 1 && valueArray[3] == 0) {
        adjustableSpeed(130, 200);
        //Serial.println("adjust 2");
      }
    }
  }





  // Serial.print(valueArray[0]);
  // Serial.print(" ");
  // Serial.print(valueArray[1]);
  // Serial.print(" ");
  // Serial.print(valueArray[2]);
  // Serial.print(" ");
  // Serial.print(valueArray[3]);
  // Serial.println(" ");
}
