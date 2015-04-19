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

// TMP36 sensor at pin A0
const int sensor = A0;

// Arduino sketch setup
void setup() {

  Serial.begin(9600);

  dht.begin();

  lcd.begin(16, 2);
 
}

// Arduino sketch loop
void loop() {

  // Wait for sensor
  delay(2000);

  // Temperature (ºC) and humidity (%)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Calculate the temperature of TMP36 in celsius
  int valorSensor = analogRead(sensor);
  float voltaje = (valorSensor / 1024.0) * 5.0;
  float temperatura = (voltaje - .5) * 100;

  // Print the temperature on the LCD
  lcd.clear();

  lcd.setCursor(0,0);  
  lcd.print("T: ");
  lcd.print(temperatura); // TMP36, use t for the DHT11
  lcd.print((char)223); // º symbol
  lcd.print("C");

  lcd.setCursor(0,1);  
  lcd.print("H: ");
  lcd.print(h);
  lcd.print("%");
  
}

