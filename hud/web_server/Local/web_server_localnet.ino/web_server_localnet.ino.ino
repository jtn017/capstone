
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*------------------------------------------------------------------------------
 CONFIGURE ADAFRUIT OLED
------------------------------------------------------------------------------*/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Replace with your network credentials
const char* ssid     = "OnePlus8Pro";
const char* password = "70a8d2c12d5e";

// Set up the server object
//ESP8266WebServer server;
ESP8266WebServer server;

/*
float range = 0.0;
float velocity = 0.0;
float gas_milage = 0.0;
*/

String n_name;
float lat = 0.0;
float lon = 0.0;
float n_speed;
float dir;
float dist_next_step = 0.0;

// Keep track of the sensor data that's going to be sent by the client
float sensor_value = 0.0;

void setup() {
  
  // Print IP Address as a sanity check
  Serial.begin(115200);
  Serial.println();
  
  // Configure the server's routes
  server.on("/",handleIndex); // use the top root path to report the last sensor value
  server.on("/update",handleUpdate); // use this route to update the sensor value

  server.on("/name",handleName); // use this route to update the sensor value
  server.on("/lat",handleLat); // use this route to update the sensor value
  server.on("/lon",handleLon); // use this route to update the sensor value
  server.on("/speed",handleSpeed); // use this route to update the sensor value
  server.on("/dir",handleDir); // use this route to update the sensor value
  server.on("/dist_next_step",handleDistToNext); // use this route to update the sensor value
  server.on("/reset",handleReset); // use this route to update the sensor value
  
  server.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP Address: "); 
  Serial.println(WiFi.localIP());

//  handleReset();

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setRotation(0);
  //display.flip();
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
  String ip_addr = WiFi.localIP().toString();
  display.print(ip_addr);
  display.println();
  display.display();

  //display.print(String(dist_next_step));
  //display.println();
  //display.display();

  //Serial.println(WiFi.localIP());
  
}
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void handleIndex() {
  server.send(200,"text/plain",String(sensor_value)); // we'll need to refresh the page for getting the latest value
}

void handleUpdate() {
  // The value will be passed as a URL argument
  sensor_value = server.arg("value").toFloat();
  Serial.println(sensor_value);
  server.send(200,"text/plain","Updated\n");
}

void handleName(){
  n_name = server.arg("value");
  Serial.println("Updating Name with " + String(n_name));
  server.send(200, "text/plain", "Name Updated\n");
  displayall();
  Serial.println("Updated Name!");
}

void handleLat(){
  lat = server.arg("value").toFloat();
  Serial.println("Updating Latitude with " + String(lat));
  server.send(200, "text/plain", "Latitude Updated\n");
  displayall();
  Serial.println("Updated Latitude!");
}

void handleLon(){
  lon = server.arg("value").toFloat();
  Serial.println("Updating Longitude with " + String(lon));
  server.send(200, "text/plain", "Longitude Updated\n");
  displayall();
  Serial.println("Updated Longitude!");
}

void handleSpeed(){
  n_speed = server.arg("value").toFloat();
  Serial.println("Updating Speed with " + String(n_speed));
  server.send(200, "text/plain", "Speed Updated\n");
  displayall();
  Serial.println("Updated Speed!");
}

void handleDir(){
  dir = server.arg("value").toFloat();
  Serial.println("Updating directions with " + String(dir));
  server.send(200, "text/plain", "Directions Updated\n");
  displayall();
  Serial.println("Updated Directions!");
}

void handleDistToNext(){
  dist_next_step = server.arg("value").toFloat();
  Serial.println("Updating dist to next step with " + String(dist_next_step));
  server.send(200, "text/plain", "Dist. to Next Step Updated\n");
  displayall();
  Serial.println("Updated Dist. to Next Step!");
}

void handleReset(){
  display.clearDisplay();
  display.print(String(""));
  display.println();
  display.print(String(""));
  display.println();
  display.print(String(""));
  display.println();
  display.print(String(""));
  display.println();
  display.print(String(""));
  display.println();
  display.print(String(""));
  display.println();
    
  display.display();
}

void displayall(void){

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setRotation(0);
  //display.flip();
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.print(String(n_name));
  display.println();
  display.print(String(lat));
  display.println();
  display.print(String(lon));
  display.println();
  display.print(String(n_speed));
  display.println();
  display.print(String(dir));
  display.println();
  display.print(String(dist_next_step));
  display.println();
    
  display.display();
    
}
