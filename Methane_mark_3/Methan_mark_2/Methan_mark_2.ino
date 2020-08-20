
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
RF24 radio(9,10); // initializes radio for coms


const byte killSwitch = 3; // 
float RS_air;
int sensorPin= A0;
float CO2Val;
SCD30 carbonSensor;
int redPin = 8;
int greenPin = 9;
volatile byte kill = LOW;


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
  tone(5, melody[0], 500);
  delayMicroseconds(1000);
  noTone(5);
  }



void setup() {
  Serial.begin(9600); 
  
  pinMode(redPin, OUTPUT);
  pinMode(killSwitch, INPUT);// sets modes for LEDs and Buttons
  pinMode(greenPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(killSwitch), depress, RISING); // creates the interrupt routine for button press
  sensorValue = analogRead(A0); 
  carbonSensor.begin();
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("CO2 Levels: ");
  lcd.setCursor(0,2);
  lcd.print("Methane Levels: ");


  
  radio.begin()
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.enableDynamicPayloads();
  radio.powerUp();

}

void loop() {
  int currTime = millis();
  String CO2 = "CO2: ";
  String methane = "CH4: ";
  CO2 += String(CO2Val);
  methane += String(R0);
  radio.
  if(CO2Val < 5000 && R0 < 5000){
    kill = 0;
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    if(carbonSensor.dataAvailable()){ // Gets the carbon dioxide values and prints to LCD
      CO2Val = carbonSensor.getCO2();
     }
     sensorValue = analogRead(A0); // this takes in the value from the methane sensor
     sensorVolt = sensorValue * (5.0 / 1023.0);
     RS_air = ((5.0 * 10.0) / sensorVolt) - 10.0; // alters the analog voltage into PPM
     R0 = RS_air / 4.4;
     if(currTime % 1000 == 0){ // prints time every 1000 milliseconds ( 1 second) TODO
        lcd.clear();
        lcd.print("CO2 Levels: ");
        lcd.setCursor(0,2);
        lcd.print("Methane Levels: ");
        lcd.setCursor(0,1); // prints CO2 Values
        lcd.print(CO2Val);
        lcd.print(" PPM");
     
        lcd.setCursor(0,3);
        lcd.print(R0);
        lcd.print(" PPM"); // prints methane values
     }
  }
  else{
    int start = millis();
    if(kill == 0){
      if(currTime % 500 == 5){
        tone(5, melody[0], 500);
        int notoneTime = currTime;
        if(currTime == notoneTime+500){
            noTone(5);
        }
        
        
        }
    }
    if(currTime % 1000 == 0 && kill == 0){
      Serial.print("there");
      Serial.print('\n');
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      lcd.setCursor(0,0);
      lcd.print("PPM LEVELS TOO HIGH");
      lcd.setCursor(0,1);
      lcd.print("CHECK EQUIPMENTS");
      lcd.setCursor(0,2);
      lcd.print("Press Button to");
      lcd.setCursor(0,3);
      lcd.print("disable alarm");
      Serial.print(kill);
      delay(100);
      }
      else if (kill == 1){
        if(carbonSensor.dataAvailable()){ // Gets the carbon dioxide values and prints to LCD
           CO2Val = carbonSensor.getCO2();
     }
        sensorValue = analogRead(A0); 
        sensorVolt = sensorValue * (5.0 / 1023.0);
        RS_air = ((5.0 * 10.0) / sensorVolt) - 10.0; 
        R0 = RS_air / 4.4;
        lcd.clear();
        lcd.print("Alarm silenced");
        lcd.setCursor(0,1);
        lcd.print("reset at acceptable PPM");
        lcd.setCursor(0,2);
        lcd.print("CO2: ");
        lcd.print(CO2Val);
        lcd.setCursor(0,3);
        lcd.print("Methane: ");
        lcd.print(R0);
        delay(100);
        }
  }
  radio.write(&CO2, sizeof(CO2));
  radio.write(&methane, sizeof(methane));
  
}

void depress() {
     // beep();
      kill = HIGH;
  
  }
