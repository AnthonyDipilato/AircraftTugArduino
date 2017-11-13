/*    Aircraft Tug Configuration
 *   
 *    Settings and pin configurations
 *  
 */

#ifndef configuration_h
#define configuration_h

// Relays
#define RELAY_HORN            23
#define RELAY_LIGHTS          25
#define RELAY_STROBE          27
#define RELAY_HITCH           29
// H-Bridge
#define MOTOR_L_C1            47
#define MOTOR_L_C2            49
#define MOTOR_L_PWM           2
#define MOTOR_R_C1            51
#define MOTOR_R_C2            53
#define MOTOR_R_PWM           3

// Min pwm percent for motor running
#define PWM_MIN               85

// Motor safety cuttoff
// Time between commands to reset motors to 0
// to prevent runaway tug on disconnect
#define COMMAND_TIMEOUT       1500  // command intervals are 500 or on change so 3 missed intervals should be safe cutoff

// Horn timeout
// horn works on momentary button timeout prevents sticky buttons on disconnect
#define HORN_TIMEOUT          1000 // no reason for a constant horn blast longer than a second

// Debounce delay for checking buttons
#define DEBOUNCE_DELAY        100
// Command Addresses
// Relays


#endif

