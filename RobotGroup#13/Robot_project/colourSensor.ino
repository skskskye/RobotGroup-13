
int avgRed = 0;
int avgGreen = 0;
int avgBlue = 0;

void colorSensor() {
  tcs3200 tcs(S0, S1, S2, S3, out);




  int threshhold = 2;
  const int samples = 5;

  long sumRed = 0;
  long sumGreen = 0;
  long sumBlue = 0;
  for (int i = 0; i < samples; i++) {
    sumRed += tcs.colorRead('r');
    sumGreen += tcs.colorRead('g');
    sumBlue += tcs.colorRead('b');
  }

  avgRed = sumRed / samples;
  avgBlue = sumBlue / samples;
  avgGreen = sumGreen / samples;

  Serial.print("R = ");
  Serial.print(avgRed);
  Serial.print(" G = ");
  Serial.print(avgGreen);
  Serial.print(" B = ");
  Serial.println(avgBlue);
}



String colorReading() {
  if (avgRed > 30 && avgBlue > 30 && avgBlue > 30) {
    return "white";
  }

  if (avgBlue < 8 && avgGreen < 8 && avgRed < 8) {
    return "black";
  }



  if (avgRed > 20 && avgGreen < 10 && avgBlue > 9) {
    return "red";
  } else if (avgRed > 45 && avgGreen > 20 && avgBlue > 14) {
    return "yellow";
  } else if (avgBlue > 20) {
    return "blue";
  }
}