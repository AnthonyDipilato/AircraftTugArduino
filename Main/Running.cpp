/*  Run Class
 *   
 *   
 */

 #include "Arduino.h"
 #include "Running.h"
 #include "Configuration.h"


Running::Running(){
  // initial values
  lastCommandR  = 0;
  lastCommandL  = 0;
  hornStart     = 0;
  // booleans for relays
  hornState     = false;
  lightState    = false;
  strobeState   = false;
  hitchState    = false;
  hitchCurrent  = false;
  currentSpeedR = 0;
  currentSpeedL = 0;
 }

// for the wheel hitch method should only be called directly to unlock
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
      hitchCurrent = state; 
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

void Running::setHitch(bool state){
  Serial.print("setHitch: true");
  hitchState = state;
  // if mag switch is disabled
  if(!ENABLE_MAG_SWITCH){
    digitalWrite(RELAY_HITCH, LOW);
    hitchCurrent = true;
  }
}

void Running::lockHitch(){
  // if how hitch is set to lock but not yet locked we will lock it 
  if(hitchState && !hitchCurrent){ 
    digitalWrite(RELAY_HITCH, LOW); // LOW is on
    hitchCurrent = true;
  }
}


byte Running::getOutputValue(int value){
  float output;
  // soften top angles from unintentionally turning we will round at 75%
  if(value < -75){ value = -100;}
  if(value > 75){ value = 100;} 
  value = abs(value);
  output = ((float) value / 100) * 255;
  return (byte) output;
}

void Running::setTargetSpeed(int address, int value){
  // value should be in the range -100,100
  if(value < -100){ value = -100;}
  if(value > 100){ value = 100;}
  // left motor
  if(address == MOTOR_L_ADDRESS){
    lastCommandL = currentTime;
    targetSpeedL = value;
    Serial.print("New TargetL: ");
    Serial.println(targetSpeedL);
  }
  if(address == MOTOR_R_ADDRESS){
    lastCommandR = currentTime;
    targetSpeedR = value;
    Serial.print("New TargetR: ");
    Serial.println(targetSpeedR);
  }
}

// set pwm output to motors
void Running::setMotorSpeed(int address, int value){
  byte output = getOutputValue(value);
  int motor1; int motor2;
  if(address == MOTOR_L_ADDRESS){
    // grab pwm values
    motor1 = MOTOR_L_PWM1;
    motor2 = MOTOR_L_PWM2;
    // update the motor's current value
    currentSpeedL = value;
  }
  else if(address == MOTOR_R_ADDRESS){
    // grab pwm values
    motor1 = MOTOR_R_PWM1;
    motor2 = MOTOR_R_PWM2;
    // update the motor's current value
    currentSpeedR = value;
  }
  else{ // invalid address
    Serial.println("Invalid Address");
    return;
  }
  // check if forward of reverse
  if(value > 0){
    analogWrite(motor1, output);
    analogWrite(motor2, 0);
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

// Immediately stop both motors
void Running::allStop(){
  Serial.println("ALL STOP");
  setMotorSpeed(5, 0);
  setMotorSpeed(6, 0);
}

// Safety function to avoid runaway motors
void Running::motorCheck(){ 
  if(targetSpeedL == 0 && targetSpeedR == 0){
    return;
  }
  if(currentTime > (lastCommandR + COMMAND_TIMEOUT) && targetSpeedR != 0){
    Serial.println("Timeout: Stop Right");
    targetSpeedR = 0;
  }
  if(currentTime > (lastCommandL + COMMAND_TIMEOUT) && targetSpeedL != 0){
    Serial.println("Timeout: Stop Left");
    targetSpeedL = 0;
  }
}

// Function to avoid sticky horn
void Running::hornCheck(){ 
  if(hornState && currentTime > hornStart + HORN_TIMEOUT)
    toggleRelay(RELAY_HORN, false);
}

// Checks if motors are at target speed;
void Running::motorLoop(){
  // delay motor update
  int newSpeed;
  if(currentTime > motorUpdate + MOTOR_DELAY){
    // Check left motor
    if(currentSpeedL > targetSpeedL){ // lower speed
      Serial.print("CurrentL: ");
      Serial.print(currentSpeedL);
      Serial.print(" TargetL: ");
      Serial.println(targetSpeedL);
      newSpeed = currentSpeedL - MOTOR_INCREMENT;
      if(newSpeed < targetSpeedL){ newSpeed = targetSpeedL;}
      setMotorSpeed(MOTOR_L_ADDRESS, newSpeed);
    }
    if(currentSpeedL < targetSpeedL){ // lower speed
      Serial.print("CurrentL: ");
      Serial.print(currentSpeedL);
      Serial.print(" TargetL: ");
      Serial.println(targetSpeedL);
      newSpeed = currentSpeedL + MOTOR_INCREMENT;
      if(newSpeed > targetSpeedL){ newSpeed = targetSpeedL;}
      setMotorSpeed(MOTOR_L_ADDRESS, newSpeed);
    }
    // Check right motor
    if(currentSpeedR > targetSpeedR){ // lower speed
      Serial.print("CurrentR: ");
      Serial.print(currentSpeedR);
      Serial.print(" TargetR: ");
      Serial.println(targetSpeedR);
      newSpeed = currentSpeedR - MOTOR_INCREMENT;
      if(newSpeed < targetSpeedR){ newSpeed = targetSpeedR;}
      setMotorSpeed(MOTOR_R_ADDRESS, newSpeed);
    }
    if(currentSpeedR < targetSpeedR){ // lower speed
      Serial.print("CurrentR: ");
      Serial.print(currentSpeedR);
      Serial.print(" TargetR: ");
      Serial.println(targetSpeedR);
      newSpeed = currentSpeedR + MOTOR_INCREMENT;
      if(newSpeed > targetSpeedR){ newSpeed = targetSpeedR;}
      setMotorSpeed(MOTOR_R_ADDRESS, newSpeed);
    }
    // set timeout
    motorUpdate = currentTime;
  }
}

void Running::loop_(){
  currentTime = millis();
  motorCheck();   // Safety Check
  hornCheck();    // Check for horn timeout
  motorLoop();
}

