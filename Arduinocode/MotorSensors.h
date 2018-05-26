#ifndef _MOTORSENSORS_
#define _MOTORSENSORS_

#include <LiquidCrystal.h> //Include Liquid Crystal Library
#include <HX711.h> //Include HX711 Load Cell library
#include "HX711.h" //Include Load Cell header file
#include <math.h> //Include math library

#define calibration_factor -8815 //set calibration factor
#define DAT 3 //define DAT pin
#define CLK 2 //define CLK pin
#define pressurePin A1
#define tempPin A2

HX711 scale(DAT,CLK); 
LiquidCrystal lcd(8,9,4,5,6,7); //Define LCD pins

//Initialize variables
float maxkg=0; 
float currentval=0;
float maxforce=0;
char mode;
char lastmode;
float MaxPressure=0;
float maxTemp=0;

void LoadCellSetup()
{
  scale.set_scale(calibration_factor); //Set scale with calibration factor
  scale.tare(); //Tare scale
  //Print menu of options
  Serial.println("Press 0 to begin testing");
  Serial.println("Press 1 to end testing");
  Serial.println("Press t to tare");
  //Print to LCD screen
  lcd.begin(16,2);
  lcd.print("Maximum Force:");
}

void LoadCellLCDSetup()
{
  scale.set_scale(calibration_factor); //Set scale with calibration factor
  scale.tare(); //Tare scale
}

void LoadCellRun()
{
  currentval=scale.get_units(); //Read force
  Serial.print("Reading (kg): ");
  Serial.println(currentval); //Print reading in serial monitor
}

void LivePressure()
{
  float rawReading=analogRead(pressurePin);
  Serial.print("Raw Reading: ");
  Serial.println(rawReading);
  float pressure=400*((5*rawReading/1023)-0.5);
  if(pressure>MaxPressure)
  {
    MaxPressure=pressure;
  }
  Serial.print("Pressure: ");
  Serial.println(pressure);
}

void LoadCell()
{
  
  if(Serial.available())
  {
    mode=Serial.read(); //Read user input
  }
  
  if(mode=='0')
  {
    Serial.print("Reading (kg): ");
    Serial.println(currentval); //Print reading in serial monitor
    lastmode=mode;
    if(currentval>maxkg)
    {
      maxkg=currentval; //Set maximum value
      maxforce=maxkg*9.8; //Convert to Newtons
    }
  }
  if(mode=='1' && mode!=lastmode)
  {
    Serial.print("Max Force Detected: ");
    Serial.print(maxforce); //Print max force in serial monitor
    Serial.println(" N");
    lcd.setCursor(0,1); //Move LCD cursor to first position in second row
    lcd.print(maxforce); //Print max force to LCD screen
    lcd.setCursor(7,1); //Set cursor to 7th position in second row
    lcd.print("N"); //Add units
    lastmode=mode;
  }
  if(mode=='t')
  {
    scale.set_scale(calibration_factor); //Reset and tare scale
    scale.tare();
  }
}

float LoadCellRunLCD()
{
  currentval=scale.get_units();
  if(currentval>maxkg)
  {
    maxkg=currentval; //Set maximum value
    maxforce=maxkg*9.8; //Convert to Newtons
  }
  return maxforce; 
}

float PressureMax()
{
  float rawReading=analogRead(pressurePin);
  float pressure=400*((5*rawReading/1023)-0.5);
  if(pressure>MaxPressure)
  {
    MaxPressure=pressure;
  }
  return MaxPressure;
}

float Thermistor() 
{
  int raw=analogRead(tempPin);
  float resistor = 13800; //value of fixed resistor in Vernier shield
  float resistance; //create local variable for resistance
  float temp; //create local variable for temperature

  resistance=log(resistor*raw/(1023-raw)); //calculate resistance
  temp = 1 / (0.00102119 + (0.000222468 * resistance) + (0.000000133342 * resistance * resistance * resistance)); //calculate temperature using the Steinhart-Hart equation
  temp = temp - 273.15; //Convert Kelvin to Celsius     
  temp = (temp * 1.8) + 32.0; // Celsius to Fahrenheit                  
  if(temp>maxTemp)
  {
    maxTemp=temp;
  }
  return maxTemp; //return the temperature
}

#endif
