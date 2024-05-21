tcs3200 tcs(S0, S1, S2, S3, out);

int avgRed = 0;
int avgGreen = 0;
int avgBlue = 0;

int threshhold = 2;


void colorSensor() {

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

String colorReading(){
  if(avgRed > avgGreen + threshhold && avgRed > avgBlue + threshhold){
    return "red";
  }else if (avgGreen > avgRed + threshhold && avgGreen > avgBlue + threshhold){
    return "green";
  }else if(avgBlue > avgRed + threshhold && avgBlue > avgGreen + threshhold){
    return "blue";
  }
}