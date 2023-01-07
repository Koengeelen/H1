// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,8,9,10,11,12);

int VRx = A0;
int VRy = A1;
int SW = 2;

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

void setup()
{
// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();

  Serial.begin(9600); 
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 

}

void loop(){
  delay(600);
  lcd.clear();
  lcd.setCursor(0,0);
  
    xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  SW_state = digitalRead(SW);
  mapX = map(xPosition, 0, 1023, -512, 512);
  mapY = map(yPosition, 0, 1023, -512, 512);

if(mapX < -500){
  lcd.print("left");
  }
  else if(mapX > 500){
     lcd.print("right");
  }
   if(mapY < -500){
    lcd.print("Up");
  }
  else if(mapY > 500){
     lcd.print("Down");
  }
  if (SW_state == 0){
    lcd.print("Click");
  }
  delay(100);
  


  
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
