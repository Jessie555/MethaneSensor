
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "SparkFun_SCD30_Arduino_Library.h"
#include "pitches.h"
Servo shoulder;
LiquidCrystal_I2C lcd(0x27, 20, 4);
float sensorValue;
float sensorVolt;
float R0;

int killSwitch = 6; // not yet implemented
float RS_air;
int sensorPin= A0;
float CO2Val;
SCD30 carbonSensor;
int redPin = 8;
int greenPin = 9;
bool kill= 0;


int melody[] = { // note that the buzzer has
  NOTE_DS8
};
int noteDurations[] = {
  1
};
void triggerAlarm(){ // triggers the alarm
  for(int i =0; i < 8; i++){ 
    int noteDuration = 1000 / noteDurations[i];
    tone(5, melody[0], noteDuration);
    int pause = noteDuration * 1.30;
    delay(pause);
    noTone(5);
    }
  }
void beep(){ // user feedback for pressing button
  tone(5, melody[0], 1000);
  noTone(5);
  }



void setup() {
  Serial.begin(9600); 
  pinMode(redPin, OUTPUT);
  pinMode(killSwitch, INPUT); // sets modes for LEDs and Buttons
  pinMode(greenPin, OUTPUT);
  sensorValue = analogRead(A0); 
  carbonSensor.begin();
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("CO2 Levels: ");
  lcd.setCursor(0,2);
  lcd.print("Methane Levels: ");

}

void loop() {
  int currTime = millis();
  if(CO2Val < 5000 && R0 < 5000){
    kill = 0;
    if(carbonSensor.dataAvailable()){ // Gets the carbon dioxide values and prints to LCD
     // lcd.setCursor(0, 1);
      CO2Val = carbonSensor.getCO2();
     // lcd.print(CO2Val);
      //lcd.print(" PPM");
     }
     sensorValue = analogRead(A0); // this takes in the value from the methane sensor
     //sensorValue = sensorValue / 500;
     sensorVolt = sensorValue * (5.0 / 1023.0);
     RS_air = ((5.0 * 10.0) / sensorVolt) - 10.0; // alters the analog voltage into PPM
     R0 = RS_air / 4.4;
     if(currTime % 1000 == 0){ // prints time every 1000 milliseconds ( 1 second)
        lcd.clear();
        lcd.print("CO2 Levels: ");
        lcd.setCursor(0,2);
        lcd.print("Methane Levels: ");
        lcd.setCursor(0,1); // prints CO2 Values
        lcd.print(CO2Val);
     //Serial.print(CO2Val);
         Serial.print('\n');
         Serial.print(sensorValue);
         lcd.print(" PPM");
        // delay(100);
     
         lcd.setCursor(0,3);
         lcd.print(R0);
         lcd.print(" PPM"); // prints methane values
        // delay(100);
     }
  }
  else{
    lcd.clear();
    if(kill == 0){
      if(currTime % 500 == 0){
        tone(5, melody[0], 500);
        
        
        }
    }
    if(currTime % 1000 == 0){
      lcd.setCursor(0,0);
      lcd.print("PPM LEVELS TOO HIGH");
      lcd.setCursor(0,1);
      lcd.print("CHECK EQUIPMENTS FOR LEAKS");
      lcd.setCursor(0,2);
      lcd.print("Press Button to disable alarm");
      if(digitalRead(killSwitch) == HIGH); {// disables alarm
        beep();
        kill = 1;
        lcd.clear();
        lcd.print("Alarm silenced");
        lcd.setCursor(0,1);
        lcd.print("System will reset at acceptable PPM");
        lcd.setCursor(0,2);
        lcd.print("CO2: ");
        lcd.print(CO2Val);
        lcd.setCursor(0,3);
        lcd.print(R0);
      }
    }
  }
}
