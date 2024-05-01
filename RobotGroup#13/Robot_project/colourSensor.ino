void colorSensor(){
  //for red

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  int redFreq = pulseIn(out, LOW);

  Serial.print(redFreq);
  Serial.print(" ");

  // //for green
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
}