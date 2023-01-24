int ir;// variable to store the value of the sensor



void setup()

{

Serial.begin(9600);// initializing the Serial communication

pinMode(7, INPUT);// assigning the sensor as a input to Arduino

}

void loop(){

  ir=digitalRead(7);// reading the output of the sensor

  if(ir==LOW){// if the sensor detects the any reflected radiation
  Serial.println("LOW");// turn on the LED
  }
  else if(ir==HIGH){// if the sensor detects the any reflected radiation
  Serial.println("HIGH");// turn on the LED
  }

}
