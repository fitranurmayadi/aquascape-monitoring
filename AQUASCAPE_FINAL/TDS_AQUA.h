#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A2
GravityTDS gravityTds;

float tdsValue = 0;

void tds_init(){
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
}

    
