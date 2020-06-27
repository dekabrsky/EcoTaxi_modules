#include <SoftwareSerial.h>
SoftwareSerial btSerial(D2, D3); 
unsigned long previousMillis = 0; 
const long interval = 500;  
void setup() {
  delay(1000);
  Serial.begin(9600);     
  btSerial.begin(9600);    
  pinMode(D4, OUTPUT);
  Serial.println("Started...");
}
void loop() {
  if (btSerial.available() > 0) {    
    char data = btSerial.read();  
    switch (data)
    {
      default:
        break;
    }
  }
  unsigned long currentMillis = millis();
  
  }
}
