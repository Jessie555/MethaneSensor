

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
bool reset= 0;


int melody[] = {
  NOTE_DS8
};
int noteDurations[] = {
  1
};


void triggerAlarm(){
  for(int i =0; i < 8; i++){
    int noteDuration = 1000 / noteDurations[i];
    tone(5, melody[0], noteDuration);
    int pause = noteDuration * 1.30;
    delay(pause);
    noTone(5);


    
  }
  
  
  }


void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(killSwitch, INPUT);
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
  triggerAlarm();
  // read the state of the pushbutton value:
  if(digitalRead(killSwitch)== 1){
    kill = true;
    }
  if(CO2Val < 5000 && R0 < 5000){
    digitalWrite(greenPin,HIGH);
    
    }
    else if(kill == false){
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);
      triggerAlarm();
      } 
     else {
      lcd.clear();
      lcd.print("Killswitch Activated");
      while(reset == false){
        lcd.setCursor(0,1);
        lcd.print("Click button to reset values");
        if(digitalRead(killSwitch) == true){
          reset = true;
          CO2Val = 0;
          R0 = 0;
          break;
          }
        
        }
      }

  
  if(carbonSensor.dataAvailable()){
    lcd.setCursor(0, 1);
    CO2Val = carbonSensor.getCO2();
    lcd.print(CO2Val);
    delay(100);
    lcd.print(" PPM");
  }
  
  for(int i =0; i < 500; i++){
    sensorValue += analogRead(A0);
    }
   sensorValue = sensorValue / 500;
   sensorVolt = sensorValue * (5.0 / 1023.0);
   RS_air = ((5.0 * 10.0) / sensorVolt) - 10.0;
   R0 = RS_air / 4.4;
  

  lcd.setCursor(0,3);
  lcd.print(R0);
  lcd.print(" PPM");
  delay(1500);
  kill = 0;
  reset = 0;

}
