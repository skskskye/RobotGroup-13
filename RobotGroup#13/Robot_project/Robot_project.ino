//libs
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

//bools
bool isTurningRight = false;
bool isTurningLeft = false;
bool isTurningAround = false;
bool isMovingTurn = false;
bool adjustTurn = false;

//timings
unsigned long irMillis = 0;
unsigned long motorMillis = 0;
unsigned long ultrasonicMillis = 0;
unsigned long turnTime = 0;
unsigned long moveTime = 0;


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
  Serial.begin(9600);

  //onboard leds
  WiFiDrv::pinMode(redLED, OUTPUT);
  WiFiDrv::pinMode(greenLED, OUTPUT);
  WiFiDrv::pinMode(blueLED, OUTPUT);



  //connect to internet
  while (!Serial)
    ;

  Serial.println("attempting to connect");

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    WiFiDrv::analogWrite(redLED, 255);
    WiFiDrv::analogWrite(greenLED, 0);
    WiFiDrv::analogWrite(blueLED, 0);
    Serial.print(".");
    delay(5000);
  }

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
  //millis
  unsigned long currentMillis = millis();
  int* valueArray = readInfrared();
  // if (currentMillis - ultrasonicMillis >= 50) {
  //   ultrasonicMillis = currentMillis;
  //   Serial.println("time!");
  //   if (ultrasonicDist() <= 7 || isTurningAround == true) {
  //     Serial.println(ultrasonicDist());
  //     Serial.println(isTurningAround);
  //     if (isTurningAround) {
  //       if (currentMillis - turnTime >= 50) {
  //         if (valueArray[1] == 1 || valueArray[2] == 1) {
  //           isTurningAround = false;
  //           stop();
  //         }
  //       }
  //     } else {
  //       turnAround();
  //       isTurningAround = true;
  //       turnTime = currentMillis;
  //     }
  //   }
  // }





  //turn sequence
  if (isTurningAround == false) {
    if ((valueArray[0] == 0 && valueArray[1] == 1 && valueArray[2] == 1 && valueArray[3] == 1) || isTurningLeft == true && isTurningRight == false) {
      isTurningLeft = true;
      

      if(!isMovingTurn){
        adjustableSpeed(255, 255);
        delay(250);
      }
      
  
      isMovingTurn = true;

      valueArray = readInfrared();
      if (valueArray[1] == 1 || valueArray[2] == 1) {
        adjustableSpeed(255, -255);
        delay(40);
        stop();
        delay(250);
        isMovingTurn = false;
        isTurningLeft = false;
        adjustTurn = true;
      }else if(isMovingTurn){
        adjustableSpeed(-155, 155);
      }

      

    } else if ((valueArray[0] == 1 && valueArray[1] == 1 && valueArray[2] == 1 && valueArray[3] == 0) || isTurningRight == true && isTurningLeft == false) {
      isTurningRight = true;

      if(!isMovingTurn){
        adjustableSpeed(255, 255);
        delay(250);
      }
      

      isMovingTurn = true;

      valueArray = readInfrared();
      if (valueArray[1] == 1 || valueArray[2] == 1) {
        adjustableSpeed(-255, 255);
        delay(40);
        stop();
        delay(250);
        isTurningRight = false;
        isMovingTurn = false;
      }else if(isMovingTurn){
        adjustableSpeed(155, -155);
      }
    }

    //infared data
    if (currentMillis - irMillis >= 2) {
      irMillis = currentMillis;
      if (valueArray[0] == 0 && valueArray[1] == 1 && valueArray[2] == 1 && valueArray[3] == 0) {
        adjustableSpeed(200, 200);
      } else if (valueArray[0] == 0 && valueArray[1] == 1 && valueArray[2] == 0 && valueArray[3] == 0) {
        adjustableSpeed(200, 140);
        Serial.println("adjust 1");
      } else if (valueArray[0] == 0 && valueArray[1] == 0 && valueArray[2] == 1 && valueArray[3] == 0) {
        adjustableSpeed(140, 200);
        Serial.println("adjust 2");
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
