#ifndef _MOTORTESTINGH_
#define _MOTORTESTINGH_

/*---------------------------- LCD main menu ----------------------------*/
#include "MotorSensors.h"

//#include <LiquidCrystal.h>    //Include LCD library
//
// LiquidCrystal lcd (8, 9, 4, 5, 6, 7);  
int menuMode=0;
int lastVal=0;
int currentVal=0;
int sensorValue=0;
int Value=0;
int newReading=0;
float peakForce=0;
float peakPressure=0;
float peakTemp=0;

int btn=A0;

void InitializeMotorTest()
 {  
 /*----------------Start up screen----------------*/
  lcd.begin (16, 2);          //Number of columns and rows
  lcd.setCursor (0, 0);       //Print message at 0, 0 position of LCD
  lcd.print ("Project 2:");   //Print Menu message onto LCD
  lcd.setCursor (0, 1);       // Set cursor to 0 column, 1st row
  lcd.print ("Rocket Stand"); //Print message: Rocket test stand 
  delay (5000);               //Delay for 5 seconds before going 
  /*-----------------------------------------------*/
  lcd.clear ();               // Clear LCD before displaying menu options

  lcd.setCursor (0, 0);
  lcd.print("Choose mode with");
  lcd.setCursor(0,1);
  lcd.print("select button");
  LoadCellLCDSetup();
  Serial.begin(9600);
  pinMode (btn, INPUT);   //Reading voltage values corresponding from the buttons on LCD shield
 }

/*
Select: 720
Left: 480
Down: 307
Up: 131
Right: 0
*/

void MotorTest()
 {
  sensorValue=analogRead(btn);
  
  if(sensorValue==720 && lastVal!=9)
  {
    menuMode++;
    lastVal=9;
  }
  
  if(sensorValue>721 || sensorValue<720)
  {
    lastVal=0;
  }
  
  switch(menuMode)
  {
    case 0:
    //lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Choose mode with");
    lcd.setCursor(0,1);
    lcd.print("select button");
    break;
    
    case 1:
    lcd.setCursor(0,0);
    lcd.print("Load Cell Cal   ");
    lcd.setCursor(0,1);
    lcd.print("Use right 2 tare");
    newReading=analogRead(btn);
//    Serial.print("New Reading: ");
//    Serial.println(newReading);
    if(newReading==0 && Value!=13)
    {
      menuMode=11;
      Value=13;
    }
    break;

    case 2:
    lcd.setCursor(0,0);
    lcd.print("Max Force       ");
    lcd.setCursor(0,1);
    lcd.print(peakForce);
    lcd.setCursor(5,1);
    lcd.print("         ");
    break;

    case 11:
    scale.set_scale(calibration_factor);
    scale.tare();
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Tare Successful ");
    lcd.setCursor(0,1);
    lcd.print("Return with left");
    //Value=8;
    newReading=analogRead(btn);
    LoadCellRun();
    if(newReading==480 && Value!=9)
    {
      menuMode=1;
      Value=9;
    }
    break; 
    
    case 3:
    lcd.setCursor(0,0);
    lcd.print("Thermistor      ");
    lcd.setCursor(0,1);
    lcd.print(peakTemp);
    lcd.setCursor(5,1);
    lcd.print("         ");
    break;

    case 4:
    lcd.setCursor(0,0);
    lcd.print("Pressure        ");
    lcd.setCursor(0,1);
    lcd.print(peakPressure);
    lcd.setCursor(5,1);
    lcd.print("         ");
    break;

    case 5:
    lcd.setCursor(0,0);
    lcd.print("Up to begin test");
    newReading=analogRead(btn);
    if(newReading==131 && Value!=11)
    {
      menuMode=7;
      Value=11;
    }
    break;

    case 7:
    LoadCellRun();
    LivePressure();
    lcd.setCursor(0,0);
    lcd.print("Test in progress");
    lcd.setCursor(0,1);
    lcd.print("Down to end test");
    newReading=analogRead(btn);
    peakForce=LoadCellRunLCD();
    peakPressure=PressureMax();
    peakTemp=Thermistor();
    if(newReading==307 && Value!=10)
    {
      menuMode=10;
      Value=10;
    }
    break;

    case 10:
    lcd.setCursor(0,0);
    lcd.print("Testing Complete");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(5000);
    menuMode=2;
    break;
    
    default:
    menuMode=0;
    break;
  }
}

#endif
