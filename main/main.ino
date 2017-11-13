/*
 *
 *  Aircraft Tug
 *  Author: Anthony DiPilato
 *  anthonydipilato.com
 *
 *  This project is for the Aircraft Tug that I built.
 *  You can see the build log and details at [url]
 *
 *  The arduino acts as slave to the iOS app.
 *  Inputs:
 *  iOS commands via bluetooth HC-08
 *
 *  Outputs:
 *  4 relays
 *  PWM pins for dual channel h-bridge to control motors
 *
 *  Relays:
 *   Horn
 *   Flood lights
 *   Warning strobe
 *   Actuator for wheel hitch
 *   
 */

#include "configuration.h"


char inData[8];
int index = 0;

void setup()
{
  // Serial output for debugging and bluetooth module
  Serial.begin(9600);
  // Relays Pins
  pinMode(RELAY_HORN,   OUTPUT); 
  pinMode(RELAY_LIGHTS, OUTPUT); 
  pinMode(RELAY_STROBE, OUTPUT); 
  pinMode(RELAY_HITCH,  OUTPUT);
  // H-Bridge Pins
  pinMode(MOTOR_L_C1,   OUTPUT); 
  pinMode(MOTOR_L_C2,   OUTPUT); 
  pinMode(MOTOR_L_PWM,  OUTPUT); 
  pinMode(MOTOR_R_C1,   OUTPUT);
  pinMode(MOTOR_R_C2,   OUTPUT);
  pinMode(MOTOR_R_PWM,  OUTPUT);
  // Initialize relays (HIGH is off)
  digitalWrite(RELAY_HORN,    HIGH);
  digitalWrite(RELAY_LIGHTS,  HIGH);
  digitalWrite(RELAY_STROBE,  HIGH);
  digitalWrite(RELAY_HITCH,   HIGH);
  // Initialize H-Bridge
  // C pins control direction
  // C1,C2 = HIGH,HIGH stop LOW,HIGH forward HIGH,LOW reverse
  digitalWrite(MOTOR_L_C1,    HIGH);
  digitalWrite(MOTOR_L_C2,    HIGH);
  digitalWrite(MOTOR_R_C1,    HIGH);
  digitalWrite(MOTOR_R_C2,    HIGH);
  // speed is controlled by PWM (analog)
  analogWrite(MOTOR_L_PWM, 0);
  analogWrite(MOTOR_R_PWM, 0);
}


void checkSerial(){

    boolean response = false;
    while(Serial.available() > 0){
       response = true;
       char input = Serial.read();
       Serial.print(input);
    }
    if(response)
      Serial.println("-");
    
}

void loop()
{

  checkSerial();
  
}


