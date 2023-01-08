#include <Arduino.h>
#include <Wire.h>

  float t1 = 20;
  float t2 = 20;
  float t3 = 20;
  float t4 = 20;

  float p1 = 1;
  float p2 = 1;


  float rpm = 230;
  float flow = 0.01;

  float Thrtl = 0;

  float fuel = 100;

  unsigned long totaltime = 0;

void setup() {
  // Initialize serial communication at a baud rate of 9600
  Serial.begin(9600);
 
}




void loop() {
  // Read a sensor value
  unsigned long time0 = millis();
  
  // Send the sensor value over the serial connection
  delay(200);

  //Serial.print("Temp:");      //Temp1
  Serial.print(t1);
  float value = (float)random(1001) / 10000;
  t1 = t1 + value;
  Serial.print("    ");

                                //Temp2
  Serial.print(t2);
  float value10 = (float)random(1001) / 10000;
  t2 = t2 + value10;
  Serial.print("    ");

                                //Temp3
  Serial.print(t3);
  float value2 = (float)random(1001) / 10000;
  t3 = t3 + value2;
  Serial.print("    ");

                                //Temp4
  Serial.print(t4);
  float value3 = (float)random(1001) / 10000;
  t4 = t4 + value3;
  Serial.print("    ");

                                //P1
  Serial.print(p1);
  float value4 = (float)random(1001) / 100000;
  p1 = p1 + value4;
  Serial.print("    ");

                                //P2
  Serial.print(p2);
  float value5 = (float)random(1001) / 10000;
  p2 = p2 + value5;
  Serial.print("    ");

 // Serial.print("Flow:");
  Serial.print(flow);
  float value6 = (float)random(1001) / 100000;
  flow = flow + value6;
  Serial.print("    ");
  
  //Serial.print("RPM:");        //RPM
  Serial.print(rpm);
  float value7 = (float)random(1001) / 1000;
  rpm = rpm + value7;
  Serial.print("    ");

  Serial.print(Thrtl);
  float value8 = (float)random(1001) / 1000;
  Thrtl = Thrtl + value8;
  Serial.print("    ");
 
  Serial.print(fuel);
  float value9 = (float)random(1001) / 100000;
  fuel = fuel - value9;
  Serial.print("    ");

  unsigned long time1 = millis();

  totaltime = totaltime + time1 - time0;

  //Serial.print("Time:");
  Serial.print(totaltime);
  Serial.println("    ");

}