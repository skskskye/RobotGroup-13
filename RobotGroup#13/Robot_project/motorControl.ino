void forward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("move");
}

void turnAround(){
  analogWrite(in1, 150);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  analogWrite(in4, 50);
}

void backward(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void adjustableSpeed(int speed1, int speed2, String dir){
  if(dir == "left"){
    analogWrite(in1, speed1);
    analogWrite(in2, LOW);
    analogWrite(in3, LOW);
    analogWrite(in4, speed2);
  }else if(dir == "right"){
    analogWrite(in1, LOW);
    analogWrite(in2, speed1);
    analogWrite(in3, speed2);
    analogWrite(in4, LOW);
  }else if(dir == "straight"){
    analogWrite(in1, LOW);
    analogWrite(in2, speed1);
    analogWrite(in3, LOW);
    analogWrite(in4, speed2);
  }
}



void stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}