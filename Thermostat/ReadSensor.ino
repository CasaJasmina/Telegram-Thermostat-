#include "DHT.h"
#include "config.h"

DHT dht(DHTPIN, DHTTYPE);

Sensor::Sensor(void) {
}

bool  Sensor::ReadSensors() {
  h = dht.readHumidity();
  t = dht.readTemperature(); // Read temperature as Celsius (the default)
  f = dht.readTemperature(true);  // Read temperature as Fahrenheit (isFahrenheit = true)
  
  if (isnan(h) || isnan(t) || isnan(f)) {             // Check if any reads failed and exit early (to try again).
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }

  return true;
}

void Sensor::begin() {
  dht.begin();
}

float  Sensor::GetTemp() {
#ifdef USE_fahrenheit
  return f;
#else
  return t;
#endif
}

float Sensor::GetHumidity() {
  return h;
}
