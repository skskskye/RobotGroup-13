int* readInfrared() {
  static int arr[4];

  arr[0] = digitalRead(ir1);
  arr[1] = digitalRead(ir2);
  arr[2] = digitalRead(ir3);
  arr[3] = digitalRead(ir4);
  return arr;
}