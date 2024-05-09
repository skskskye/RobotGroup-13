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

void adjustableSpeed(int speed1, int speed2){
  
  speed1 = max(-255, min(255, speed1));
  speed2 = max(-255, min(255, speed2));
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