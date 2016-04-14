#include <Arduino.h>
#include <sht1xalt.h>

#define dataPin 10
#define clockPin 11

#define clockPulseWidth 1

#define supplyVoltage sht1xalt::VOLTAGE_5V

#define temperatureUnits sht1xalt::UNITS_FAHRENHEIT

sht1xalt::Sensor sensor( dataPin, clockPin, clockPulseWidth,
                         supplyVoltage, temperatureUnits );

                    
float oldTemp;
float lowTemp = 10000;
float highTemp;
int ledYellow = 7;
int ledRed = 5;
const int flexpin = 0; 

void setup() {
  Serial.begin(38400);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  Serial.println("Starting up (2s delay)");
  delay(2000);
  
  sensor.configureConnection();
  sensor.softReset();
}

void loop() {
  float temp;
  float rh;
  int flexposition; 
  sht1xalt::error_t err;

  flexposition = analogRead(flexpin);
  
  delay(2000);
  
  err = sensor.measure(temp, rh);
  if (err) {
    switch (err) {
      case sht1xalt::ERROR_NO_ACK:
        Serial.println("SHT1x failed to acknowledge a command!");
        break;
      case sht1xalt::ERROR_MEASUREMENT_TIMEOUT:
        Serial.println("SHT1x failed to produce a measurement!");
        break;
    }
    sensor.softReset();
    return;
  }

  Serial.print("sensor: ");
  Serial.print(flexposition);

  Serial.println("");
  Serial.print("The temperature is ");
  Serial.print(temp);
  if (sensor.getUnits() == sht1xalt::UNITS_CELCIUS) {
    Serial.print(" Celcius");
  } else {
    Serial.print(" Fahrenheit");
  }
  Serial.print(", the relative humidity is ");
  Serial.print(rh);
  Serial.println("%");

  if (temp <= 80){
    //Serial.println("I fucking hate programming");
    digitalWrite(ledRed, HIGH);
  }else{
    digitalWrite(ledRed, LOW);
  }
  
    Serial.print("Old temp: ");
    Serial.print(oldTemp);
    
   if (oldTemp == temp){
    Serial.println("");
    Serial.print("The temp is consistent"); 
    digitalWrite(ledYellow, HIGH);
    }else{
      digitalWrite(ledYellow, LOW);
    }

    if(highTemp < temp){
      highTemp = temp;
      Serial.println("");
      Serial.print("The new High Temp is: ");
      Serial.print(highTemp);
    }

    if(lowTemp > temp){
      lowTemp = temp;
      Serial.println("");
      Serial.print("The new Low Temp is: ");
      Serial.print(lowTemp);
    }
    
   oldTemp = temp;  

   Serial.println("");
   Serial.println("");

}