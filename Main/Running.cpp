/*  Run Class
 *   
 *   
 */

 #include "Arduino.h"
 #include "Running.h"
 #include "Configuration.h"


Running::Running(){
  // initial values
  lastUpdateL     = 0;
  lastUpdateR     = 0;
  hornStart       = 0;
  motorLPercent   = 0;
  motorLDirection = 0;
  motorRPercent   = 0;
  motorRDirection = 0;
  // booleans for relays
  hornState   = false;
  lightState  = false;
  strobeState = false;
  hitchState  = false;
  motorLState = false;
  motorRState = false; 
 }

void Running::toggleRelay(int relay, boolean state){
  int pin;
  // match relay with constants and toggle state
  switch(relay){
    case(RELAY_HORN):
      hornState = state;
      if(state) hornStart = millis();
      break;
    case(RELAY_LIGHTS):
      lightState = state;
      break;
    case(RELAY_STROBE):
      strobeState = state;
      break;
    case(RELAY_HITCH):
      hitchState =  state;
      break;
  }
  if(state){
    digitalWrite(relay, LOW); // LOW is on
  }else{
    digitalWrite(relay,HIGH); // HIGH is off
  }
}

void Running::setMotorDirection(int address, int value){
    int c1 = 0;
    int c2 = 0;
    // if not forward or backward apply brakes (0,0)
    if(value > 0){
      c1 = 1;
    }
    else if(value < 0){
      c2 = 1;
    }
    // left motor
    if(address == 5){
      digitalWrite(MOTOR_L_C1, c1);
      digitalWrite(MOTOR_L_C2, c2);
      lastUpdateL = millis();
    }
    // right motor
    else if(address == 6){
      digitalWrite(MOTOR_R_C1, c1);
      digitalWrite(MOTOR_R_C2, c2);
      lastUpdateR = millis();
    }
}

void Running::setMotorSpeed(int address, int value){
  float output;
  // set motor direction
  setMotorDirection(value, address);
  value  = abs(value);
  if(value > 100) value = 100;
  output = ((float) value / 100) * 255;
  outputValue = (byte) output;
  // 5 is left 6 is right
  if(address == 5){
    analogWrite(MOTOR_L_PWM, outputValue);
  }else if(address == 6){
    analogWrite(MOTOR_R_PWM, outputValue);
  }
}

void Running::allStop(){
  setMotorSpeed(5, 0);
  setMotorSpeed(5, 0);
}

void Running::motorCheck(){ // safety function to avoid runaway motors
  if(currentTime > lastUpdateR + COMMAND_TIMEOUT || currentTime > lastUpdateL + COMMAND_TIMEOUT)
    allStop();
}

void Running::hornCheck(){ // because a sticky horn could get annoying
  if(hornState && currentTime > hornStart + HORN_TIMEOUT)
    toggleRelay(RELAY_HORN, false);
}

void Running::loop_(){
  currentTime = millis();
  motorCheck();
  hornCheck();
}

