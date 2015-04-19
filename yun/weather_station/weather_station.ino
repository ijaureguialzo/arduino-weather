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

// Parse.com
#include <Parse.h>
#include <Bridge.h>

ParseClient parse;

// Parse.com app keys
#include "parse_keys.h"

// Arduino sketch setup
void setup() {

  Serial.begin(9600);
  //while (!Serial); // wait for a serial connection

  Serial.println("Arduino Weather");

  dht.begin();

  lcd.begin(16, 2);

  // Initialize Bridge
  Bridge.begin();

  // Initialize Parse
  parse.begin(PARSE_APPLICATION_ID, PARSE_CLIENT_KEY);
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

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(t); // temperatura -> TMP36, t -> DHT11
  lcd.print((char)223); // º symbol
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print("%");

  ParseObjectCreate create;
  create.setClassName("DatosSensores");
  create.add("idEstacion", "YUN0001");
  create.add("dht11", t);
  create.add("tmp36", temperatura);
  create.add("humedad", h);
  ParseResponse response = create.send();

  Serial.println("\nResponse for saving a TestObject:");
  Serial.print(response.getJSONBody());
  if (!response.getErrorCode()) {
    String objectId = response.getString("objectId");
    Serial.print("Test object id:");
    Serial.println(objectId);
  } else {
    Serial.println("Failed to save the object");
  }
  response.close(); // Do not forget to free the resource

  delay(3000);  // Esperar 3+2=5s

}

