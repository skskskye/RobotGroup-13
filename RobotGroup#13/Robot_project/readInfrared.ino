void readInfrared() {
  int ir1State = digitalRead(ir1);
  int ir2State = digitalRead(ir2);
  int ir3State = digitalRead(ir3);
  int ir4State = digitalRead(ir4);

  Serial.print(ir1State);
  Serial.print(" ");
  Serial.print(ir2State);
  Serial.print(" ");
  Serial.print(ir3State);
  Serial.print(" ");
  Serial.print(ir4State);
  Serial.println(" ");
}