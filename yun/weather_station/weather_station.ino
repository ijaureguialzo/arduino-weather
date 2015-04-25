// LCD setup
#include <LiquidCrystal.h>

int RS = 8;
int E = 9;
int D4 = 6;
int D5 = 5;
int D6 = 3;
int D7 = 2;

LiquidCrystal lcd(RS, E, D4, D5, D6, D7 );

// DHT11 sensor using Adafruit Library
// https://github.com/adafruit/DHT-sensor-library
#include "DHT.h"

#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

// Parse.com
#include <Parse.h>
#include <Bridge.h>

ParseClient client;

// Parse.com app keys
#include "parse_keys.h"

// Arduino sketch setup
void setup() {

  Serial.begin(9600);
  Serial.println("Arduino Weather");

  dht.begin();

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arduino Weather");

  // Initialize Bridge
  Bridge.begin();

  // Initialize Parse
  client.begin(PARSE_APPLICATION_ID, PARSE_CLIENT_KEY);

  // Wait for sensor
  delay(2000);
}

// Arduino sketch loop
void loop() {

  // Temperature (ºC) and humidity (%)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the temperature on the LCD
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(t);
  lcd.print((char)223); // º symbol
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print("%");

  ParseObjectCreate parse;
  parse.setClassName("DatosSensores");
  parse.add("idEstacion", "YUN0001");
  parse.add("dht11", t);
  parse.add("humedad", h);
  parse.send();

  Serial.println("Data sent to Parse");

  delay(120000);  // Wait 2min

}

