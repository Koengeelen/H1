// include the library code:
#include <LiquidCrystal.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19,18,17,16,15,14);


#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11

//para las rpm
volatile byte rpmcount;
unsigned int rpm;
unsigned long timeold;



//Variables para la dirección del joystick
int VRx = A0;
int VRy = A1;
int SW = 4;

// Variables para la posición en a pantalla
int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;
int SC = 16;
int SL = 2;
int s1 = 0;
int s2 = 0;
int men1 = 0;
int men2 = 0;
int men = 0;
volatile int n = 0;   
int i = 0;

//Variables de tiempo para el cuentarevoluciones


//Variable para el acelerador
int thrtl = 0;

int paso [4][4] =
{
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
};

int menospaso [4][4] =
{
  {0, 0, 0, 1},
  {0, 0, 1, 0},
  {0, 1, 0, 0},
  {1, 0, 0, 0}
};
 
int stp_position = 0;
float REAC = 0;
float REAC2 = 0;
float MATH1 = 0;


//Función de imprimir el menu
void menu(){

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("1TEMP");
  lcd.setCursor(1, 1);
  lcd.print("3PRES");
  lcd.setCursor(8, 0);
  lcd.print("2THRT");
  lcd.setCursor(8, 1);
  lcd.print("4RPM");

}


void rpm_fun()
{
rpmcount++;
}



//Menú para la presión
void pressure(){
  //-700～0kPa correspond with 4.5～0.5V
    SW_state = digitalRead(SW);  
    delay(100);         //Comprobar que el valor es 0
      while (SW_state == 1){              //Loop para cuando no se hace click en el Joystick
        SW_state = digitalRead(SW);  

        float SensorOffset = 102;

        lcd.print("P1:");      
        float P1 = (analogRead(A3)-SensorOffset)/100.0;
        lcd.print(P1);                                     //Medir P1 e imprimirlo
        lcd.setCursor(0, 1);
        lcd.print("P2:");
        float P2 = (analogRead(A4)-SensorOffset)/100.0;
        lcd.print(P2);                                      //Medir P2 e imprimirlo
        lcd.setCursor(0, 0);
        lcd.clear();
        
        delay(100);

      }
}


void count(){
  //if (digitalRead(3) == LOW){
     n++;
    Serial.println("C");

  //}
 
}
 

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
      rpm = 0.6*10000/(millis() - timeold)*rpmcount;  
      timeold = millis();
      rpmcount = 0;

//Print out result to lcd


//Restart the interrupt processing
      attachInterrupt(0, rpm_fun, FALLING);


      lcd.print(rpm);
      //lcd.clear();
      lcd.setCursor(0,0);

      delay(1000);


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



//Menú para el acelerador (Falta meterle un servo)
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
    lcd.print(">");
    Serial.println(i);                    //Imprimir dicha barra
    //
    //lcd.print("%");
    }
    lcd.setCursor(0, 0);
    //Serial.println(SW_state);
    delay(100);
    lcd.clear();

  REAC = (stp_position-thrtl);

  if (REAC < 0){

    REAC2 = (abs(REAC));

  for (int b = 0; b <= REAC2; b++){
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(IN1, paso[i][0]);
      digitalWrite(IN2, paso[i][1]);
      digitalWrite(IN3, paso[i][2]);
      digitalWrite(IN4, paso[i][3]);
      delay(4);
    }
    
    //REAC2 = (abs(REAC)/(MATH1));
  }

    //stepper.step(REAC2); //positivo 
    stp_position = stp_position + REAC2;
    //Serial.print("   ");
    //Serial.print(stp_position);
  }

  if (REAC > 0){

    REAC2 = (abs(REAC));

  for (int b = 0; b <= REAC2; b++){

    for (int i = 0; i < 4; i++)
    {
      digitalWrite(IN1, menospaso[i][0]);
      digitalWrite(IN2, menospaso[i][1]);
      digitalWrite(IN3, menospaso[i][2]);
      digitalWrite(IN4, menospaso[i][3]);
      delay(4);
    }
    
    //REAC2 = (abs(REAC)/(MATH1));
  }
  
      stp_position = stp_position - REAC2;
      //Serial.print("   ");
    //Serial.print(stp_position);
}

  if (REAC == 0){

 stp_position = thrtl;

  }




  }
}



void setup()
{
// set up the LCD's number of columns and rows:
  lcd.begin(SC, SL);
  lcd.clear();

  Serial.begin(9600); 

  attachInterrupt(0, rpm_fun, RISING);

  rpmcount = 0; 
  rpm = 0;
  timeold = 0;
  
  pinMode(VRx, INPUT);
  pinMode(5, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 

  sensors.begin(); 

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  for (int b = 0; b < 100; b++){

    for (int i = 0; i < 4; i++)
    {
      digitalWrite(IN1, menospaso[i][0]);
      digitalWrite(IN2, menospaso[i][1]);
      digitalWrite(IN3, menospaso[i][2]);
      digitalWrite(IN4, menospaso[i][3]);
      delay(4);
    }
 }

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

        //Serial.print(s1);
        //Serial.print(" || ");
        //Serial.println(s2);

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
  


  
 /*lcd.setCursor(0,0); // Sets the cursor to col 0 and row 0
 lcd.print("Hello world");
 delay(3000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("This works right");
 delay(3000);
 lcd.clear();
*/

/*lcd.print("SensorVal1: "); // Prints Sensor Val: to LCD
lcd.print(analogRead(potPin1)); // Prints value on Potpin1 to LCD
lcd.setCursor(0,1); // Sets the cursor to col 1 and row 0
lcd.print("SensorVal2: "); // Prints Sensor Val: to LCD
lcd.print(analogRead(potPin2)); // Prints value on Potpin1 to LCD
*/
}
