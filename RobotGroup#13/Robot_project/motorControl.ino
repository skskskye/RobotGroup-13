void forward(){ //forward command
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("move");
}

void turnAround(){ //turnaround command
  analogWrite(in1, 150);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  analogWrite(in4, 50);
}

void backward(){ //backward command
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

float mult = 1.25; //mult for the motor speed

void adjustableSpeed(int speed1, int speed2){ // adjustable speed function, can cahnge the speed of each motor
  

  //applying mult
  speed1 = speed1 * mult;
  speed2 = speed2 * mult;

  //creating a range from 255 to -255 for the speed
  speed1 = max(-255, min(255, speed1));
  speed2 = max(-255, min(255, speed2));

  //applying the speed to the motors
  analogWrite(in1, speed1 > 0 ? speed1 : 0);
  analogWrite(in2, speed1 < 0 ? -speed1 : 0);
  analogWrite(in3, speed2 > 0 ? speed2 : 0);
  analogWrite(in4, speed2 < 0 ? -speed2 : 0);
}



void stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}