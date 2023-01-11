
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>


// setup del onewire para las temperaturas
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//setup de la pantalla
LiquidCrystal lcd(8,9,7,6,5,4);

//Pines del joystick
int VRx = A0; int VRy = A1; int SW = 3;

//Variables para la posición en el menú de la pantalla
int xPosition = 0; int yPosition = 0; int SW_state = 0;
int mapX = 0; int mapY = 0;
int SC = 16; int SL = 2;
int s1 = 0; int s2 = 0;
int men1 = 0; int men2 = 0; int men = 0;
volatile int n = 0;   
int i = 0;

//Variables para el acelerador
int thrtl = 0;
float REAC = 0;
float REAC2 = 0;
float MATH1 = 0;

Servo servoMotor;


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

//loop para las revoluciones con la interrupción
void rpm_fun(){rpmcount++;}

void pressure(){
    SW_state = digitalRead(SW);
    delay(100);
    while (SW_state == 1){
        SW_state = digitalRead(SW);
        float SensorOffset = 102;

        lcd.print("P1:");
        float P1 = (analogRead(A3)-SensorOffset)/100;
        lcd.print(P1);
        lcd.setCursor(0, 1);
        lcd.print("P2:");
        float P2 = (analogRead(A4)-SensorOffset)/100;
        lcd.print(P2);
        lcd.setCursor(0, 1);
        delay(100);
        lcd.clear();
    }
}


void count(){n++;Serial.println("C");}

//Menú para el menú de las RPM
void Rpm(){

    //delay(100);
    
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

//Print out result to lcd


//Restart the interrupt processing
      attachInterrupt(0, rpm_fun, FALLING);


      delay(100);

      lcd.print(rpm);
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
    lcd.print("T1:");                           
    lcd.print(sensors.getTempCByIndex(0));      //Medir T1 e imprimirlo
    lcd.setCursor(0, 1);
    lcd.print("T2:");
    lcd.print(sensors.getTempCByIndex(1));      //Medir T2 e imprimirlo
    lcd.setCursor(0, 0);

    //Serial.println(SW_state);
    delay(100);
  }
}


void Throttle(){

  SW_state = digitalRead(SW);
  while (SW_state == 1){
    SW_state = digitalRead(SW);           //Abrir el menú acelerador hasta que se clique el botón del Joystick
    lcd.setCursor(0, 0);
    thrtl = analogRead(A2);               //Coger el valor del potenciometro
    thrtl = thrtl/(10.23);                //Transformarlo a un porcentaje (0-100)
    lcd.print("MIN    ");       
    lcd.print(thrtl);
    if (thrtl < 10){
      lcd.print(" ");
    }
    lcd.print("    MAX");
    lcd.setCursor(0, 1);

    int thrtl2 = thrtl/(6.25);            //Transformar el porcentaje a dieciseisavos para poner una barra de porcentaje de aceleración
    for (int i = 0; i <= thrtl2; i++){
    lcd.print(">");                    //Imprimir dicha barra
    //
    //lcd.print("%");
    }

    servoMotor.write(A2*1.8);

    lcd.setCursor(0, 0);
    //Serial.println(SW_state);
    delay(100);
    lcd.clear();

  }
}

void check_map_position(){
    if(mapX < -500 && s1 > 0 ){ //Left
          s1 = s1 -7;
        }
      else if(mapX > 500 && s1 < 14){ //right
          s1 = s1 + 7;
        }
      if(mapY < -500 && s2 > 0){ //up
          s2 = s2 - 1;
        }
      else if(mapY > 500 && s2 < 1){ //down
          s2 = s2 + 1;
        }
      if (SW_state == 0){
        //lcd.print("Click");
        }
        delay(200);

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
      
      men = men1+men2;
}


void setup(){
    servoMotor.attach(10);
    servoMotor.write(0);

     lcd.begin(SC, SL);
     lcd.clear();

    Serial.begin(9600); 

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


 void loop(){


    menu();
    digitalWrite(5, HIGH);
    //while(SW_state < 1){
    delay(100);
    xPosition = analogRead(VRx);
    yPosition = analogRead(VRy);
    SW_state = digitalRead(SW);
    mapX = map(xPosition, 0, 1023, -512, 512);
    mapY = map(yPosition, 0, 1023, -512, 512);

      
    lcd.setCursor(s1, s2);
    lcd.print(">");
    //lcd.blink();
    lcd.setCursor(s1, s2);

    check_map_position();

    if (SW_state == 0){
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
  }
    //lcd.noBlink();
    lcd.clear();

 }
