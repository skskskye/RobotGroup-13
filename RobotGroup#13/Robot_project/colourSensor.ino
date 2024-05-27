
//declaring the average values
int avgRed = 0;
int avgGreen = 0;
int avgBlue = 0;

void colorSensor() {
  tcs3200 tcs(S0, S1, S2, S3, out); //intializing TCS




  const int samples = 5; //amount of smples to use


  //initital sum of all colours
  long sumRed = 0;
  long sumGreen = 0;
  long sumBlue = 0;

  //sampling the colours
  for (int i = 0; i < samples; i++) {
    sumRed += tcs.colorRead('r');
    sumGreen += tcs.colorRead('g');
    sumBlue += tcs.colorRead('b');
  }


  //getting the average of the colours
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

  //checking if the colour it detected is white
  if (avgRed > 20 && avgBlue > 20 && avgBlue > 20) {
    //Serial.println("white");
    return 'w';
  }


  //checking if the colour is black
  if (avgBlue < 20 && avgGreen < 20 && avgRed < 20) {
    //Serial.println("black");
    return 'n'; //n = none, aka black
    
  }




  //checking if its red
  if (avgRed > 21 && avgGreen < 10 && avgBlue > 9) {
   // Serial.println("red");
    return 'r';
  
  //checking if its yellow
  } else if (avgRed > 45 && avgGreen > 20 && avgBlue > 14) {
    //Serial.println("yellow");
    return 'y';
  
  //checking if its blue
  } else if (avgBlue > 24 && avgGreen < 20 && avgRed < 20) {
    //Serial.println("blue");
    return 'b';

  //checking if its green
  }else if(avgRed > 15 && avgRed < 30 && avgGreen > 25 && avgBlue < 25){
    return 'g';
  
  //fail safe if all fails, just say its black
  }else{
    return 'n';
  }
}