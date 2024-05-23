
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



char colorReading() {
  if (avgRed > 20 && avgBlue > 20 && avgBlue > 20) {
    //Serial.println("white");
    return 'w';
  }

  if (avgBlue < 20 && avgGreen < 20 && avgRed < 20) {
    //Serial.println("black");
    return 'n'; //n = none, aka black
    
  }



  if (avgRed > 21 && avgGreen < 10 && avgBlue > 9) {
   // Serial.println("red");
    return 'r';
  } else if (avgRed > 45 && avgGreen > 20 && avgBlue > 14) {
    //Serial.println("yellow");
    return 'y';
  } else if (avgBlue > 24 && avgGreen < 20 && avgRed < 20) {
    //Serial.println("blue");
    return 'b';
  }else if(avgRed > 15 && avgRed < 30 && avgGreen > 25 && avgBlue < 25){
    return 'g';
  }else{
    return 'n';
  }
}