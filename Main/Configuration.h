    /*    Aircraft Tug Configuration
 *   
 *    Settings and pin configurations
 *  
 */

#ifndef configuration_h
#define configuration_h

// Relays
#define RELAY_HORN            47
#define RELAY_LIGHTS          49
#define RELAY_STROBE          51
#define RELAY_HITCH           53
// H-Bridge
#define MOTOR_L_PWM1          11
#define MOTOR_L_PWM2          12
#define MOTOR_L_EN1           26
#define MOTOR_L_EN2           28
#define MOTOR_R_PWM1          9
#define MOTOR_R_PWM2          10
#define MOTOR_R_EN1           30
#define MOTOR_R_EN2           32

// Motor Addresses
#define MOTOR_R_ADDRESS       5
#define MOTOR_L_ADDRESS       6

// Motor Softening
// Higher the number the slower the acceleration
#define MOTOR_DELAY           50
#define MOTOR_INCREMENT       5


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

