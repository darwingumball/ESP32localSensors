# ESP32localSensors
ESP32 connected sensors accessible from a locally hosted web server. A starting point for those wanting to access sensor information online. The WiFi for this program is in AP mode meaning the ESP32 is its own WIFI router, but some changes with the Arduino IDE WiFi.h library can make it STA (open to the internet). Current example code is based on a DHT22 and Ultrasonic Sensor connection.

Project Slides: https://docs.google.com/presentation/d/1wJy0IBnr84VOwXi_1TIPrDMs-WctXfCHtK9N8TYMUds/edit?usp=sharing

client.println("<script>setTimeout(function() { location.reload(true); }, 3000);</script>"); //javascript addition to refresh page every 3 seconds

To add other Sensors, make a function that collects current Data when called, then put it in this chunk so that it displays next to other sensors:
            client.println("<p><h4>Distance: ");
            client.println(readDistance());
            client.println("<p><h6>Temperature: ");
            client.println(readTemperature());
            client.println("<p><h7>Humidity: ");
            client.println(readHumidity());

The HTML is making a static webpage that only updates its info on refresh. Serial monitor output will let you know of client connections and simple debugging with the WiFi code, although connection info is not accurate since the webpage is refreshed automatically.
