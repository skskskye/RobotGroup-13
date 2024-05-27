float ultrasonicDist() {

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); //getting the amount of tmie it took for trigpin to respond

  float distance = duration * 0.034 / 2; //calc for distance to be changed from time to distance

  return distance; //return the distance
}