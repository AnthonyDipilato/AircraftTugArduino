    /*    Aircraft Tug Configuration
 *   
 *    Settings and pin configurations
 *  
 */

#ifndef configuration_h
#define configuration_h

// Relays
#define RELAY_HORN            22  // #1
#define RELAY_LIGHTS          24  // #2
#define RELAY_STROBE          26  // #3
#define RELAY_HITCH           28  // #4
// H-Bridges
// Left Motor
#define MOTOR_L_PWM1          11
#define MOTOR_L_PWM2          12
#define MOTOR_L_EN1           30
#define MOTOR_L_EN2           32
// Right Motor
#define MOTOR_R_PWM1          9
#define MOTOR_R_PWM2          10
#define MOTOR_R_EN1           34
#define MOTOR_R_EN2           36

// Motor Addresses
#define MOTOR_R_ADDRESS       5
#define MOTOR_L_ADDRESS       6

// Motor Softening
// Higher the number the slower the acceleration
#define MOTOR_DELAY           50
#define MOTOR_INCREMENT       5

// Electromagnet Locks include reed switches
// so we will use them to lock on contact 
// This is a safety feature to keep them from slaming together
#define ENABLE_MAG_SWITCH     false
#define MAG_LOCK_SWITCH_L     2
#define MAG_LOCK_SWITCH_R     3

// Motor safety cuttoff
// Time between commands to reset motors to 0
// to prevent runaway tug on disconnect
#define COMMAND_TIMEOUT       250  // command intervals are 500 or on change so 3 missed intervals should be safe cutoff

// Horn timeout
// horn works on momentary button timeout prevents sticky buttons on disconnect
#define HORN_TIMEOUT          2000 

// Debounce delay for checking comands
#define DEBOUNCE_DELAY        100
// Command Addresses
// Relays


#endif

