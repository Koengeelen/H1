#include <EasyBuzzer.h>
 
void sonidoTerminado(){
  Serial.println("Sonido terminado");
}
 
void setup() {
  Serial.begin(9600);
  
  // Configuración del pin
  EasyBuzzer.setPin(50);
 
  Serial.println("Comienza el sonido");
  
  // Configuración del beep
  EasyBuzzer.beep(1000, 100, 100, 3, 300, 1, sonidoTerminado);
}

 

 // el beep se pone: (Frec, Duracion, Pausa, Reps, duracioin pausa, funcion que llama cuando termina)
void loop() {
  // Función para que funcione la librería
  EasyBuzzer.update();
}