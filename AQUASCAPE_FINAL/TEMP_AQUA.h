#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 3;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

//need to initiation
//  sensors.begin();

float temperature = 0;

float readTemperature(){
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  return temperatureC;
}
