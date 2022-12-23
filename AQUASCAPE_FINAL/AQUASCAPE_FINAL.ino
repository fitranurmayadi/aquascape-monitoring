#include "DO_AQUA.h"
#include "PH_AQUA.h"
#include "TDS_AQUA.h"
#include "TEMP_AQUA.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include <Servo.h>

Servo myservo;

String dataToSend = "";
unsigned long lastTimer = 0;
unsigned long timerRead = 600000;

void setup() {
  Serial.begin(115200);
  sensors.begin();            // inisiasi sensor suhu
  ph.begin();                 // inisiasi sensor ph
  tds_init();                 // inisiasi sensor tds
  //inisisasi servo
  myservo.attach(8);
  myservo.write(150);
  lcd.begin();                 // inisiasi lcd
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MONITOR AQUASCAPE");
  lcd.setCursor(0, 2);
  lcd.print("MEDAN 2021");
  readAllSensor();
}

void readAllSensor() {
  //turunkan servo
  //baca semua sensor dengan waktu steady selama 120 detik
  //kirim data secara serial
  //turunkan sensor
  myservo.attach(8);
  for(int x = 150; x>=50;x--){
    myservo.write(x);
    delay(20);
  }
  myservo.detach();
  for (int x = 0; x <= 120; x++) {
    delay(1000);
    temperature = readTemperature();

    ADC_Raw = analogRead(DO_PIN);
    ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;
    DO = readDO(ADC_Voltage, temperature) / 1000;

    voltage = analogRead(PH_PIN) / 1024.0 * 5000; // read the voltage
    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation

    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DO   = ");
    lcd.print(DO);
    lcd.print(" mg/L");
    lcd.setCursor(0, 1);
    lcd.print("PH   = ");
    lcd.print(phValue);
    lcd.setCursor(0, 2);
    lcd.print("TDS  = ");
    lcd.print(tdsValue);
    lcd.print(" ppm");
    lcd.setCursor(0, 3);
    lcd.print("TEMP = ");
    lcd.print(temperature);
    lcd.print(" *C");
  }
  dataToSend = String(DO) + "," + String(phValue) + "," + String(tdsValue) + "," + String(temperature);
  Serial.println(dataToSend);
  //angkat sensor
  myservo.attach(8);
  for(int x = 50; x<=150;x++){
    myservo.write(x);
    delay(20);
  }
  myservo.detach();
}

void loop()
{
  if ((millis() - lastTimer) > timerRead) {
    lastTimer = millis();
    readAllSensor();
  }
}
