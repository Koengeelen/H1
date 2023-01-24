
int test = 0;

void setup() {
  Serial.begin(9600); 
  // start the serial communication
}

void loop() {
  int Pressure = analogRead(A0); // read the analog input on pin A0
  
  test++;
  Serial.print(test);
  Serial.print("->");
  Serial.println(Pressure);
   // print the value to the serial monitor
  delay(100);
}