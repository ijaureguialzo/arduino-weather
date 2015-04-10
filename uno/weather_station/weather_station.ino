#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

// Download the Plotly library from https://github.com/plotly/arduino-api
#include "plotly_streaming_ethernet.h"

/* 
  Personal keys for Plotly

  Create a file named plotly_keys/plotly_keys.h in
  arduino libraries folder and define needed values
  
  Get the values at https://plot.ly/settings/api

  #define PLOTLY_USERNAME "<your username>"
  #define PLOTLY_API_KEY "<your api key>"
  #define PLOTLY_TOKEN1 "<your token>"
  
*/
#include "plotly_keys.h"

// LCD pin setup for Arduino Uno with Ethernet shield
int RS = 8;
int E = 9;
int D4 = 6;
int D5 = 5;
int D6 = 3;
int D7 = 2;

// LCD init
LiquidCrystal lcd(RS, E, D4, D5, D6, D7 );

// TMP36 sensor at pin A0
const int sensor = A0;

// Plotly setup
#define nTraces 1
char *tokens[nTraces] = {PLOTLY_TOKEN1}; // One token per sensor pin
plotly graph = plotly(PLOTLY_USERNAME, PLOTLY_API_KEY, tokens, "temperatura", nTraces);

// Ethernet shield setup
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x4F, 0xA2 };
IPAddress my_ip(192, 168, 1, 123);

// Try to get an IP address using DHCP or fallback to an static IP
void startEthernet() {
  Serial.println("... Initializing ethernet");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("... Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, my_ip);
  }
  Serial.println("... Done initializing ethernet");
  delay(1000);
}

// Arduino sketch setup
void setup() {

  // Log to serial console
  Serial.begin(9600);

  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.print("Temp: ?");

  // Networking up
  startEthernet();

  // Plotly graph setup
  graph.world_readable = false;
  graph.timezone = "Europe/Madrid";
  graph.maxpoints = 86400;
  graph.fileopt = "extend";

  bool success = graph.init();
  if (!success) {
    while (true) {}
  }

  graph.openStream();

}

// Arduino sketch loop
void loop() {

  // Calculate the temperature of TMP36 in celsius
  int valorSensor = analogRead(sensor);
  float voltaje = (valorSensor / 1024.0) * 5.0;
  float temperatura = (voltaje - .5) * 100;

  // Print the temperature on the LCD
  lcd.clear();
  lcd.print("Temp: ");
  lcd.print(temperatura);

  // Send the temperature value to Plotly
  graph.plot(millis(), temperatura, tokens[0]);

  delay(1000);
}

