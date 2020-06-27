#include <ESP8266WiFi.h>
String apiKey = "4KY06SWOW2MUCG6W"; 
const char *ssid = "wifi_name"; 
const char *pass = "wi-fi_password";
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
void loop()
{
  float h = analogRead(A0);
  if (isnan(h))
  {
    Serial.println("No Mq-2 signal");
    return;
  }
 
  if (client.connect(server, 80)) // "184.106.153.149" 
  {
    String postStr = apiKey;
    postStr += "&amp;field1=";
    postStr += String(h/1023*100);
    postStr += "r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.print("Gas Level: ");
    Serial.println(h/1023*100);
    Serial.println("Data Send to Thingspeak");
  }
  client.stop();
  Serial.println("Waiting...");
  delay(1500);
}
