/*  Run Class
 *   
 *   
 */

 #include "Arduino.h"
 #include "Running.h"
 #include "Configuration.h"

Running::Running(int test){
  // initial values
  lastCheck       = 0;
  lastUpdateL     = 0;
  lastUpdateR     = 0;
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
      break;
    case(RELAY_LIGHTS):
      lightState = state;
      break;
    case(RELAY_STROB):
      strobeState = state;
      break;
    case(RELAY_HITCH):
      hitchState =  state;
      break;
  }
  if(state){
    digitalWrite(relay, LOW);
  }else{
    digitalWrite(relay,HIGH);
  }
}

