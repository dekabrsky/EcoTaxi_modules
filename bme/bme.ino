#include <ESP8266WiFi.h>                               
#include <Wire.h>                                       
#include <Adafruit_BME280.h>                            
#include <Adafruit_Sensor.h>                           

#define SEALEVELPRESSURE_HPA (1013)                  
 
Adafruit_BME280 bme;                                    
 
const char* ssid = "wifi id";         
const char* password = "wifi pass";     
 
WiFiServer server(80);                                  
String header;
 
void setup() {
  Serial.begin(115200);                                
  bool status;
                                                       
  if (!bme.begin(0x76)) {                              
    Serial.println("No BME280 signal"); 
    while (1);                                          
  }
 
  Serial.print("Connecting to ");                       
  Serial.println(ssid);                                 
  WiFi.begin(ssid, password);                          
  while (WiFi.status() != WL_CONNECTED) {           
    delay(500);                                        
    Serial.print(".");                                 
  }
 
  Serial.println("");                                   
  Serial.println("WiFi connected.");                 
  Serial.println("IP address: ");                       
  Serial.println(WiFi.localIP());                    
  server.begin();                                  
}
 
void loop(){
  WiFiClient client = server.available();              
 
  if (client) {                                         
    Serial.println("New Client.");                     
    String currentLine = "";                            
    while (client.connected()) {                        
      if (client.available()) {                        
        char c = client.read();                        
        Serial.write(c);                               
        header += c;
        if (c == '\n') {                             
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");         
            client.println("Content-type:text/html ");
            client.println("Connection: close");        
            client.println("Refresh: 10");              
            client.println();
            
            client.println("<!DOCTYPE html><html>");   
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta charset='UTF-8'>");   
            client.println("<link rel=\"icon\" href=\"data:,\">");
                     
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
            client.println("table { border-collapse: collapse; width:40%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #0043af; color: white; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #bcbcbc; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");
            
            client.println("</style></head><body><h1>Метеостанция на BME280 и ESP8266</h1>");
            client.println("<table><tr><th>Параметр</th><th>Показания</th></tr>");
            client.println("<tr><td>Температура</td><td><span class=\"sensor\">");
            client.println(bme.readTemperature());
            client.println(" *C</span></td></tr>");    
            client.println("<tr><td>Давление</td><td><span class=\"sensor\">");
            client.println(bme.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Приблизительная высота над уровнем моря</td><td><span class=\"sensor\">");
            client.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
            client.println(" m</span></td></tr>"); 
            client.println("<tr><td>Влажность</td><td><span class=\"sensor\">");
            client.println(bme.readHumidity());
            client.println(" %</span></td></tr>"); 
            client.println("</body></html>");
            
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
