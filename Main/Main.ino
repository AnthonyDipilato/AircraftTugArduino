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

#include "Configuration.h"
#include "Running.h"
#include "Buttons.h"


char inData[8];
int index = 0;

// Setup running class
Running running;
// Setup switches for maglocks
Buttons maglock_switch_l(MAG_LOCK_SWITCH_L);
Buttons maglock_switch_r(MAG_LOCK_SWITCH_R);

void setup()
{

  // Serial output for debugging and bluetooth module
  Serial.begin(9600); // USB for debugging
  Serial1.begin(9600); // Bluetooth
  // Set timers for PWM H-Bridge maxes at 25KHz so we will set 3.92KHz
  TCCR1B = (TCCR2B & 0b11111000) | 0x02;
  TCCR2B = (TCCR2B & 0b11111000) | 0x02;
  // Relays Pins
  pinMode(RELAY_HORN,   OUTPUT); 
  pinMode(RELAY_LIGHTS, OUTPUT); 
  pinMode(RELAY_STROBE, OUTPUT); 
  pinMode(RELAY_HITCH,  OUTPUT);
  // H-Bridge Pins
  pinMode(MOTOR_L_PWM1,   OUTPUT); 
  pinMode(MOTOR_L_PWM2,   OUTPUT); 
  pinMode(MOTOR_L_EN1,  OUTPUT); 
  pinMode(MOTOR_L_EN2,  OUTPUT); 
  pinMode(MOTOR_R_PWM1,   OUTPUT);
  pinMode(MOTOR_R_PWM2,   OUTPUT);
  pinMode(MOTOR_R_EN1,  OUTPUT);
  pinMode(MOTOR_R_EN2,  OUTPUT);
  // Initialize relays (HIGH is off)
  digitalWrite(RELAY_HORN,    HIGH);
  digitalWrite(RELAY_LIGHTS,  HIGH);
  digitalWrite(RELAY_STROBE,  HIGH);
  digitalWrite(RELAY_HITCH,   HIGH);
  // Initialize H-Bridge
  digitalWrite(MOTOR_L_EN1,    HIGH);
  digitalWrite(MOTOR_L_EN2,    HIGH);
  digitalWrite(MOTOR_R_EN1,    HIGH);
  digitalWrite(MOTOR_R_EN2,    HIGH);
  // speed is controlled by PWM (analog)
  analogWrite(MOTOR_L_PWM1, 0);
  analogWrite(MOTOR_L_PWM2, 0);
  analogWrite(MOTOR_R_PWM1, 0);
  analogWrite(MOTOR_R_PWM2, 0);

}

// Get relay from address
int getRelay(int address){
  Serial.print("get relay: ");
  Serial.println(address);
  int relay;
  switch (address) {
    case 1:
      relay = RELAY_HORN;
      break;
    case 2:
      relay = RELAY_LIGHTS;
      break;
    case 3:
      relay = RELAY_STROBE;
      break;
    case 4:
      relay = RELAY_HITCH;
      break;
    default:
      relay = -1;
      break;
  }
  return relay;
}

// get address from relay
int getAddress(int relay){
  int address;
  switch(relay) {
    case RELAY_HORN:
      address = 1;
      break;
    case RELAY_LIGHTS:
      address = 2;
      break;
    case RELAY_STROBE:
      address = 3;
      break;
    case RELAY_HITCH:
      address = 4;
      break;
    default:
      address = -1;
      break;
  }
  return address;
}

/*    Commands
 *    0 - Status
 *    1 - Relay On
 *    2 - Relay Off
 *    3 - Set PWM
 *    4 - All Stop
 */

void readCommand(){
  // input format is comma seperated: command,address,value[newline]
  char* items = strtok(inData,","); // split by comma
  int command = atoi(items);  // get command and convert to integer
  items = strtok(NULL,",");   // call strok again for next token value
  int address = atoi(items);  // get value and convert to integer
  items = strtok(NULL,",");   // call strtok agin for next token value
  int value = atoi(items);    // value for command
  int relay;
  // redirect to command action
  switch(command){
    case(0): // Status
      {
        if(address == 0){
          // loop through all addresses to output all
          for(int i=1; i<=6; i++){
            getStatus(i);
          }
        }else{
          getStatus(address);
        }
        break;
      }
    case(1): // Relay On
      relay = getRelay(address);
      Serial.print("Relay: ");
      Serial.println(relay);
      if(relay == -1){
        break;
      }
      else if(relay == RELAY_HITCH){
        running.setHitch(true);
        break;
      }
      running.toggleRelay(relay, true);
      break;
    case(2): // Relay Off
      relay = getRelay(address);
      if(relay == -1){
        break;
      }
      running.toggleRelay(relay, false);
      break;
    case(3): // Set PWM
      running.setTargetSpeed(address,value);
      break;
    case(4): // All Stop
      running.allStop();
      break;
  }
  // if not status command return command for confirmation
  if(command != 0){
    // output command for confirmation
    Serial1.print(command);
    Serial1.print(",");
    Serial1.print(address);
    Serial1.print(",");
    Serial1.println(value);
  }
}

/* Output Status
 *  Addresses
 *  0 - All
 *  1 - Horn
 *  2 - Lights
 *  3 - Warning Strobes
 *  4 - Wheel Hitch
 *  5 - Left Motor
 *  6 - Right Motor
 */
void getStatus(int item){
  // statuses are updated in loop for this function only outputs current
  // output format 0,address,value
  char output[16];
  int value;
  switch(item){
    case(1):
      value = (int) running.hornState;
      break;
    case(2):
      value = (int) running.lightState;
      break;
    case(3):
      value = (int) running.strobeState;
      break;
    case(4):
      value = (int) running.hitchState;
      break;
    case(MOTOR_R_ADDRESS):
      value = (int) running.currentSpeedR;
      break;
    case(MOTOR_L_ADDRESS):
      value = (int) running.currentSpeedL;
      break;
    default:
      value = -1;
      break;
  }
  sprintf(output,"0,%d,%d",item,value);
  Serial.println(output);
  Serial1.println(output);
}


void checkSerial(){
    // Reading incoming commands
    while(Serial1.available() > 0){
      // commands are comma seperated integers: command,address,value followed by newline (\n)
      char inChar = (char)Serial1.read();
      if(inChar != '\n'){
        inData[index] = inChar;
        index++;
      }else{
        readCommand();
        memset(&inData,0,8); // clear command
        index = 0;
      }
    }
}

// Check switch states
void check_switches(){
    // check staus of switches
    maglock_switch_l.check();
    maglock_switch_r.check();
    // if both switches are closed lock the wheel hitch
    if(maglock_switch_l && maglock_switch_r){
      running.lockHitch();
    }
}

void loop()
{
  // check switches
  check_switches();
  // run loop
  running.loop_();
  // check for commands
  checkSerial();
}


