/*------------------------------------------------------------------------------
  03/06/2022
  Author: Jorge Pacheco
  File: 
  ------------------------------------------------------------------------------
  Description: 
  Simple Server implementation that listens to simple HTTP commands from
  Clients connected to its own network
------------------------------------------------------------------------------*/
// Setup the server to receive data over WiFi
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

// Configuration parameters for Access Point
char * ssid_ap = "ESP8266";
char * password_ap = "TEST1234";
IPAddress ip(192,168,1,27); // arbitrary IP address (doesn't conflict w/ local network)
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Set up the server object
ESP8266WebServer server;

String n_name;
float lat = 0.0;
float lon = 0.0;
int n_speed;
int dir;
float dist_next_step = 0.0;

//float range = 0.0;
//float velocity = 0.0;
//float gas_milage = 0.0;

// Keep track of the sensor data that's going to be sent by the client
float sensor_value = 0.0;

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip,gateway,subnet);
  WiFi.softAP(ssid_ap,password_ap);
  // Print IP Address as a sanity check
  Serial.begin(115200);
  Serial.println();
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  // Configure the server's routes
  server.on("/",handleIndex); // use the top root path to report the last sensor value
  server.on("/update",handleUpdate); // use this route to update the sensor value
  //server.on("/range",handleRange); // use this route to update the sensor value
  //server.on("/velocity",handleVelocity); // use this route to update the sensor value
  //server.on("/mileage",handleMileage); // use this route to update the sensor value
  
  server.on("/name",handleName); // use this route to update the sensor value
  server.on("/lat",handleLat); // use this route to update the sensor value
  server.on("/lon",handleLon); // use this route to update the sensor value
  server.on("/speed",handleSpeed); // use this route to update the sensor value
  server.on("/dir",handleDir); // use this route to update the sensor value
  server.on("/dist_next_step",handleDistToNext); // use this route to update the sensor value
  server.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
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
  server.send(200,"text/plain","Updated");
}

/*
void handleRange(){
  range = server.arg("value").toFloat();
  Serial.println("Updating range with " + String(range));
  server.send(200, "text/plain", "Range Updated");
  //testrange();
  displayall();
  Serial.println("Updated Range!");
}

void handleVelocity(){
  velocity = server.arg("value").toFloat();
  Serial.println("Updating velocity with " + String(velocity));
  server.send(200, "text/plain", "Velocity Updated");
  //testmph();
  displayall();
  Serial.println("Updated Velocity!");
}

void handleMileage(){
  gas_milage = server.arg("value").toFloat();
  Serial.println("Updating mileage with " + String(gas_milage));
  server.send(200, "text/plain", "Mileage Updated");
  displayall();
  Serial.println("Updated Mileage!");
}
*/

/*-------------------------------------------------------*/
void handleName(){
  n_name = server.arg("value");
  Serial.println("Updating mileage with " + String(n_name));
  server.send(200, "text/plain", "Name Updated");
  displayall();
  Serial.println("Updated Name!");
}

void handleLat(){
  lat = server.arg("value").toFloat();
  Serial.println("Updating Latitude with " + String(lat));
  server.send(200, "text/plain", "Latitude Updated");
  displayall();
  Serial.println("Updated Latitude!");
}

void handleLon(){
  lon = server.arg("value").toFloat();
  Serial.println("Updating Longitude with " + String(lon));
  server.send(200, "text/plain", "Longitude Updated");
  displayall();
  Serial.println("Updated Longitude!");
}

void handleSpeed(){
  n_speed = server.arg("value").toInt();
  Serial.println("Updating speed with " + String(n_speed));
  server.send(200, "text/plain", "Speed Updated");
  displayall();
  Serial.println("Updated Speed!");
}

void handleDir(){
  dir = server.arg("value").toFloat();
  Serial.println("Updating directions with " + String(dir));
  server.send(200, "text/plain", "Directions Updated");
  displayall();
  Serial.println("Updated Directions!");
}

void handleDistToNext(){
  dist_next_step = server.arg("value").toFloat();
  Serial.println("Updating mileage with " + String(dist_next_step));
  server.send(200, "text/plain", "Dist. to Next Step Updated");
  displayall();
  Serial.println("Updated Dist. to Next Step!");
}

void displayall(void){

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
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

/*
 * #define TESTING
void displayall(void){

  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

#ifdef TESTING
  String m_range = String(range) + " FT";
  display.print(m_range);
#else
  display.print(String(range));
#endif
  display.println();

#ifdef TESTING
  String m_velocity = String(velocity) + " MPH";
  display.print(m_velocity);
#else
  display.print(String(velocity));
#endif
  display.println();

#ifdef TESTING
  String m_gas_milage = String(gas_milage) + " GAL";
  display.print(m_gas_milage);
#else
  display.print(String(gas_milage));
#endif
  
  display.println();
  display.display();
    
}
*/
