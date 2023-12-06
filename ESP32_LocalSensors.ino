#include <WiFi.h>
#include <DHT.h>
#include <string.h>
#include <stdio.h>
#define DHTTYPE DHT22
#define DHT_PIN     26
#define TRIGGER_PIN 33
#define ECHO_PIN    35
DHT dht(DHT_PIN, DHT22);

const char* ssid     = "ESP32-Network";
const char* password = "Esp32-Password";
WiFiServer server(80);

String header;

float tempValue = 0;
float humidValue = 0;

unsigned long currentTime = millis();

unsigned long previousTime = 0;

const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  dht.begin();

  WiFi.softAP(ssid,password);
  
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}


float readTemperature() {
  float tempValue = dht.readTemperature();
  return tempValue;
}

float readHumidity() {
  float humidityValue = dht.readHumidity();
  return humidityValue;
}

float readDistance() {

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Pulse duration
  float duration = pulseIn(ECHO_PIN, HIGH);

  float distanceValue = (duration * .0343) / 2;

  return distanceValue;
}

String nearAlarm() {
float distanceValue = readDistance();
  
  String alarm = "Initial" ; 
  if (distanceValue < 50) {
  alarm = "Close";
  } else if (distanceValue < 200){
  alarm = "Medium Range";
  }
  else{
  alarm = "Far";
  }

  return alarm;
}

void loop() {
  WiFiClient client = server.available();   

  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");       
    String currentLine = "";               

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {            
        char c = client.read();            
        Serial.write(c);                   
        header += c;
        if (c == '\n') {                  
          // end of the client HTTP request, send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<script>setTimeout(function() { location.reload(true); }, 3000);</script>"); //javascript addition to refresh
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center; background-color: lightblue;}");
            client.println("h1 { font-size: 36px; }");  // Increase the font size of h1
            client.println("h2, h3{ font-size: 22px; }");
            client.println("h4, h5, h6, h7 { font-size: 17px; }");
            client.println(".button { background-color: yellowgreen; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 32px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: red;}</style></head>");
            client.println("<body><h1>ESP32 Web Acccesible Sensors</h1>");
            client.println("<p><h2>Sensor info is live, but only updates values every 3 seconds when webpage is refreshed.</h2>");
            client.println("<p><h3>Current Sensors: Ultrasonic, DHT22, </h3>");
      
            client.println("<p><h4>Distance: ");
            client.println(readDistance());
            client.println("<p><h5>Near Alarm: ");
            client.println(nearAlarm());
            client.println("<p> </p>");
            client.println("<p><h6>Temperature: ");
            client.println(readTemperature());
            client.println("<p><h7>Humidity: ");
            client.println(readHumidity());
            client.println("<p> </p>");
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
