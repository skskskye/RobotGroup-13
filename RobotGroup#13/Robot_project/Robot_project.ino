//libs
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

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

  WiFiDrv::pinMode(redLED, OUTPUT);
  WiFiDrv::pinMode(greenLED, OUTPUT);
  WiFiDrv::pinMode(blueLED, OUTPUT);

  

  while(!Serial);

  Serial.println("attempting to connect");
  
  while(WiFi.begin(ssid, password) != WL_CONNECTED){
    WiFiDrv::analogWrite(redLED, 255);
    WiFiDrv::analogWrite(greenLED, 0);
    WiFiDrv::analogWrite(blueLED, 0);
    Serial.print(".");
    delay(5000);
  }

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

}

void loop() {
  // //ir sensor states
  int ir1State = digitalRead(ir1);
  int ir2State = digitalRead(ir2);
  int ir3State = digitalRead(ir3);
  int ir4State = digitalRead(ir4);

  // Serial.print(ir1State);
  // Serial.print(" ");
  // Serial.print(ir2State);
  // Serial.print(" ");
  // Serial.print(ir3State);
  // Serial.print(" ");
  // Serial.print(ir4State);
  // Serial.println(" ");



  //color sensor data

  //for red
  /*
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  int redFreq = pulseIn(out, LOW);

  Serial.print(redFreq);
  Serial.print(" ");

  //for green
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  int greenFreq = pulseIn(out, LOW);

  Serial.print(greenFreq);
  Serial.print(" ");

  //for blue
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  int blueFreq = pulseIn(out, LOW);

  Serial.print(blueFreq);
  Serial.print(" ");

  Serial.println("");
*/
}

int ultrasonicDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1); //change later to make it not use delay
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  int distance = duration * 0.034/2;

  if(distance < 50){ //check for unreliable values
    return distance;
  }
  
}

