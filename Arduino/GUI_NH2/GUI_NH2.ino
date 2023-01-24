
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
#include <EasyBuzzer.h>


// setup del onewire para las temperaturas
#define ONE_WIRE_BUS 10
OneWire oneWire(ONE_WIRE_BUS);      //Temp pin
DallasTemperature sensors(&oneWire);

//setup de la pantalla
LiquidCrystal lcd(12,13,31,33,35,37);

//Funciones para medir el tiempo
unsigned long initial_time = 0; unsigned long final_time = 0;

//Pines del joystick
int VRx = A0; int VRy = A1; int SW = 53;

//Variables para la posición en el menú de la pantalla
int xPosition = 0; int yPosition = 0; int SW_state = 0;
int mapX = 0; int mapY = 0;
int SC = 16; int SL = 2;
int s1 = 0; int s2 = 0;
int men1 = 0; int men2 = 0; int men = 0;
volatile int n = 0;   
int i = 0;

//acelerador
float thrtl = 0; int thrtl_pin = A15; float thrtl_value = 0;

int num_pitidos = 0;

//flow y fuel
float flow_value = 0; int flow_pin = A5;
float fuel_value = 0; int fuel_pin = A6;

Servo servoMotor;

//Variables temperatura

int temp1 = 0; int temp2 = 0; int temp3 = 0; int temp4 = 0; int temp_sending_counter;

//variables de presión
int pres_pin1 = A12; int pres_pin2 = A13;
float pres1 = 0; float pres2 = 0;

//Variables para las RPM
volatile byte rpmcount;
unsigned int rpm;
unsigned long timeold;

//Función de imprimir el menu
void menu(){
  lcd.clear(); lcd.setCursor(1,0); lcd.print("1TEMP");
  lcd.setCursor(1, 1); lcd.print("3PRES"); 
  lcd.setCursor(8, 0); lcd.print("2THRT");
  lcd.setCursor(8, 1); lcd.print("4RPM");
}

void menu2(){
  lcd.clear(); lcd.setCursor(1,0); lcd.print("3PRES");
  lcd.setCursor(1, 1); lcd.print("5FUEL"); 
  lcd.setCursor(8, 0); lcd.print("4RPM");
  lcd.setCursor(8, 1); lcd.print("6FLOW");
}

//loop para las revoluciones con la interrupción
void rpm_fun(){rpmcount++;}

void pressure(){
    SW_state = digitalRead(SW);
    delay(100);
    while (SW_state == 1){
        SW_state = digitalRead(SW);

        lcd.print("P1:");
        float P1 = (((analogRead(pres_pin1))*0.0048)-0.5)*175;
        lcd.print(P1);
        lcd.print("KPa");
        lcd.setCursor(0, 1);
        lcd.print("P2:");
        float P2 = (((analogRead(pres_pin2))*0.0048)-0.5)*175;
        lcd.print(P2);
        lcd.print("KPa");
        lcd.setCursor(0, 1);
        read_send_raspberry();
        delay(100);
        lcd.clear();
    }
}


void count(){n++;}

//Menú para el menú de las RPM
void Rpm(){

    SW_state = digitalRead(SW);           //Comprobar que el valor es 0
      while (SW_state == 1){              //Loop para cuando no se hace click en el Joystick
      SW_state = digitalRead(SW);  
      

      //Don’t process interrupts during calculations
      detachInterrupt(0);
      //Note that this would be 60*1000/(millis() – timeold)*rpmcount if the interrupt
      //happened once per revolution instead of twice. Other multiples could be used
      //for multi-bladed propellers or fans
      rpm = 30*1000/(millis() - timeold)*rpmcount;  
      timeold = millis();
      rpmcount = 0;

      //Restart the interrupt processing
      attachInterrupt(0, rpm_fun, FALLING);


      delay(100);

      lcd.print(rpm);
      read_send_raspberry();
      lcd.clear();
      lcd.setCursor(0,0);

      
    }
}


//Menú para ver las temperaturas
void Temp(){

  SW_state = digitalRead(SW);
  while (SW_state == 1){
    SW_state = digitalRead(SW);                 //Abrir el menú temperatura hasta que se clique el botón del Joystick
    sensors.requestTemperatures();
    SW_state = digitalRead(SW); 
    lcd.print("T1:");   
    temp1 = sensors.getTempCByIndex(0);                     
    lcd.print(temp1);      //Medir T1 e imprimirlo
    lcd.setCursor(0, 1);

    lcd.print("T2:");
    temp2 = sensors.getTempCByIndex(1);
    lcd.print(temp2);      //Medir T2 e imprimirlo
    lcd.setCursor(8, 0);

    lcd.print("T3:");
    temp3 = sensors.getTempCByIndex(2);
    lcd.print(temp3);      //Medir T3 e imprimirlo
    lcd.setCursor(8, 1);

    lcd.print("T4:");
    temp4 = sensors.getTempCByIndex(3);
    lcd.print(temp4);      //Medir T3 e imprimirlo
    lcd.setCursor(0, 0);

    read_send_raspberry();
    delay(100);
  }
}


void Throttle(){

  SW_state = digitalRead(SW);
  while (SW_state == 1){
    SW_state = digitalRead(SW);           //Abrir el menú acelerador hasta que se clique el botón del Joystick
    lcd.setCursor(0, 0);
    thrtl = analogRead(thrtl_pin);               //Coger el valor del potenciometro
    thrtl = thrtl/(10.23);                //Transformarlo a un porcentaje (0-100)
    lcd.print("MIN  ");       
    lcd.print(thrtl);
    lcd.print("%");
    if (thrtl < 10){
      lcd.print(" ");
    }
    lcd.print("  MAX");
    
    lcd.setCursor(0, 1);

    int thrtl2 = thrtl/(6.25);            //Transformar el porcentaje a dieciseisavos para poner una barra de porcentaje de aceleración
    for (int i = 0; i <= thrtl2; i++){
    lcd.print(">");                    //Imprimir dicha barra
    //
    
    }

    servoMotor.write(A2*1.8);

    lcd.setCursor(0, 0);
    //Serial.println(SW_state);
    delay(100);
    read_send_raspberry();
    lcd.clear();

  }
}

void update_actuators(){
  servoMotor.write(A2*1.8);
  
}




void read_send_raspberry(){
  //Temperaturas

  initial_time = millis();
  temp_sending_counter++;


  if (temp_sending_counter %  8 == 0){
  temp1 = sensors.getTempCByIndex(0);  Serial.print(temp1);   Serial.print("    ");
  temp2 = sensors.getTempCByIndex(1);  Serial.print(temp2);   Serial.print("    ");
  temp3 = sensors.getTempCByIndex(2);  Serial.print(temp3);   Serial.print("    ");
  temp4 = sensors.getTempCByIndex(3);  Serial.print(temp4);   Serial.print("    ");
  } else{

  Serial.print(temp1);   Serial.print("    ");
  Serial.print(temp2);   Serial.print("    ");
  Serial.print(temp3);   Serial.print("    ");
  Serial.print(temp4);   Serial.print("    ");

  }
  
  //Presiones
  pres1 = analogRead(pres_pin1); pres1 = (pres1*0.00488-0.488)*175;  Serial.print(pres1);   Serial.print("    ");
  pres2 = analogRead(pres_pin2); pres2 = (pres2*0.00488-0.488)*175;  Serial.print(pres2);   Serial.print("    ");
  
  //Flujo
  flow_value = analogRead(flow_pin)*1.014*0.004887;  Serial.print(flow_value);   Serial.print("    ");
  
  //RPM
  
  
  //Consumo
  fuel_value = analogRead(fuel_pin);  Serial.print(fuel_value);   Serial.print("    ");

  //Acelerador
  thrtl_value = analogRead(thrtl_pin)/10.23;  Serial.print(thrtl_value);  Serial.print("    ");

  //Tiempo
  final_time = millis(); Serial.print(final_time);   Serial.print("    ");

  Serial.println(" ");
}


void check_map_position(){
    //xPosition = analogRead(VRx);
    //yPosition = analogRead(VRy);
    if(mapX < 200 && s1 > 0 ){ //Left
          s1 = s1 -7;
        }
      else if(mapX > 700 && s1 < 14){ //right
          s1 = s1 + 7;
        }
      if(mapY < 200 && s2 > 0){ //up
          s2 = s2 - 1;
        }
      else if(mapY > 700 && s2 < 3){ //down
          s2 = s2 + 1;
        }
      if (SW_state == 0){
        //lcd.print("Click");
        }
        delay(100);

      if (s1 == 0){
        men1 = 1;
      }
      else if (s1 == 7){
        men1 = 3;
      }
      else if (s1 == 12){
        men1 = 5;
      }

      if (s2 == 0){
        men2 = 0;
      }
      else if (s2 == 1){
        men2 = 1;
      }
      else if (s2 == 3){
        men2 = 10;
      }
      
      men = men1+men2;
}

void sonido(){
  num_pitidos++;
}

void Pitido_largo(){                        //Función para hacer un pitido largo
  EasyBuzzer.singleBeep(800, 100);
  delay(100);
  EasyBuzzer.stopBeep();
  delay(300);
}

void Pitido_corto(){                        //Función para hacer un pitido corto
  EasyBuzzer.singleBeep(1300, 50);
  delay(50);
  EasyBuzzer.stopBeep();
  delay(50);
}

void setup(){

    servoMotor.attach(10);
    servoMotor.write(0);

    EasyBuzzer.setPin(50);
    
    Pitido_largo();
    Pitido_largo();
    Pitido_largo();

    Pitido_corto();
    Pitido_corto();

    lcd.begin(SC, SL);
    lcd.clear();

    Serial.begin(57600); 

    attachInterrupt(0, rpm_fun, RISING);

    rpmcount = 0; 
    rpm = 0;
    timeold = 0;
  
    pinMode(VRx, INPUT);
    //pinMode(5, INPUT);
    pinMode(VRy, INPUT);
    pinMode(SW, INPUT_PULLUP); 

    sensors.begin(); 

    menu();

}

void fuel(){
  SW_state = digitalRead(SW);           //Comprobar que el valor es 0
      while (SW_state == 1){              //Loop para cuando no se hace click en el Joystick
      SW_state = digitalRead(SW); 

      fuel_value = analogRead(fuel_pin);  
       

      read_send_raspberry();
      delay(100);
      lcd.clear();
      lcd.setCursor(0,0);

      }
}
void flow(){
  SW_state = digitalRead(SW);           //Comprobar que el valor es 0
      while (SW_state == 1){              //Loop para cuando no se hace click en el Joystick
      SW_state = digitalRead(SW);

      flow_value = analogRead(flow_pin);  


      flow_value = flow_value*1.014*0.004887;

      read_send_raspberry();
      
      lcd.setCursor(0,0);
      lcd.print("Flow: ");
      lcd.print(flow_value);
      lcd.print(" cm3/s");
      delay(100);
      lcd.clear();

      }
}


 void loop(){

    if (men < 10){
    menu();
    }
    else{
    menu2();
    }

    EasyBuzzer.update();

    delay(20);
    xPosition = analogRead(VRx);
    yPosition = analogRead(VRy);
    SW_state = digitalRead(SW);

    mapX = xPosition;
    mapY = yPosition;
      
    lcd.setCursor(s1, s2);
    lcd.print(">");

    lcd.setCursor(s1, s2);

    check_map_position();

    if (SW_state == 0){
    
    Pitido_corto();

    if (men == 1){
      lcd.setCursor(0,0);
      lcd.clear();
      Temp();
      //Hacer void para temperatura
    }
    else if (men == 2){
      lcd.setCursor(0,0);
      lcd.clear();
      lcd.print("Pres");
      pressure();
      //Hacer void para presiones
    }
    else if (men == 3){
      lcd.setCursor(0,0);
      lcd.clear();
      Throttle();
      //Hacer void para acelerador
    }
    else if (men == 4){
      lcd.setCursor(0,0);
      lcd.clear();
      Rpm();
    //Hacer void para rpm
    }
    else if (men == 11){
      lcd.setCursor(0,0);
      lcd.clear();
      fuel();
    //Hacer void para rpm
    }
    else if (men == 13){
      lcd.setCursor(0,0);
      lcd.clear();
      flow();
    //Hacer void para rpm
    }
  }

    read_send_raspberry();
    lcd.clear();
    

 }
