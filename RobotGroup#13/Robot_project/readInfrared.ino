int* readInfrared() { // reading infared with an array
  static int arr[4]; //declaring how many values will be in the array


  //putting the values in the array
  arr[0] = digitalRead(ir1);
  arr[1] = digitalRead(ir2);
  arr[2] = digitalRead(ir3);
  arr[3] = digitalRead(ir4);

  return arr; //returning the array
}