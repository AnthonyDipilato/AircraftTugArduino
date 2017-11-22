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
  motorRPercent   = 0;
  // booleans for relays
  hornState   = false;
  lightState  = false;
  strobeState = false;
  hitchState  = false;
  motorLState = false;
  motorRState = false; 
 }

void Running::toggleRelay(int relay, boolean state){
  Serial.println("toggleRelay()");
  Serial.print("relay: ");
  Serial.println(relay);
  Serial.print("state: ");
  Serial.println(state);
  int pin;
  // match relay with constants and toggle state
  switch(relay){
    case(RELAY_HORN):
      hornState = state;
      if(state) hornStart = currentTime;
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
    Serial.println("ON");
    digitalWrite(relay, LOW); // LOW is on
  }else{
    Serial.println("OFF");
    digitalWrite(relay, HIGH); // HIGH is off
  }
}


byte Running::getOutputValue(int value){
  Serial.println("getOutputValue()");
  float output;
  value = abs(value);
  Serial.print("Value: ");
  Serial.println(value);
  if(value > 100) value = 100;
  output = ((float) value / 100) * 255;
  Serial.print("Output: ");
  return (byte) output;
}

void Running::setMotorSpeed(int address, int value){
  Serial.println("setMotorSpeed()");
  byte output = getOutputValue(value);
  int motor1; int motor2;
  // 5 is left 6 is right
  if(address == 5){
    motor1 = MOTOR_L_PWM1;
    motor2 = MOTOR_L_PWM2;
    motorLPercent = value;
    lastUpdateL = currentTime;
  }
  else if(address == 6){
    motor1 = MOTOR_R_PWM1;
    motor2 = MOTOR_R_PWM2;
    motorRPercent = value;
    lastUpdateR = currentTime;
  }
  else{ // invalid address
    return;
  }
  // check if forward of reverse
  if(value > 0){
    analogWrite(motor1, output);
    analogWrite(motor2, 0);
    Serial.print("analogWrite(");
    Serial.print(motor1);
    Serial.print(", ");
    Serial.print(output);
    Serial.println(");");
  }
  else if(value < 0 ){
    analogWrite(motor2, output);
    analogWrite(motor1, 0);
  }
  else{
    analogWrite(motor1, 0);
    analogWrite(motor2, 0);
  }
}

void Running::allStop(){
  Serial.println("all stop");
  setMotorSpeed(5, 0);
  setMotorSpeed(6, 0);
}

void Running::motorCheck(){ // safety function to avoid runaway motors
  if(motorLPercent == 0 && motorRPercent == 0){
    return;
  }
  if(currentTime > (lastUpdateR + COMMAND_TIMEOUT) && motorRPercent != 0){
    Serial.println("Stop Right");
    setMotorSpeed(6, 0);
  }
  if(currentTime > (lastUpdateL + COMMAND_TIMEOUT) && motorLPercent != 0){
    Serial.println("Stop Left");
    setMotorSpeed(5, 0);
  }
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

