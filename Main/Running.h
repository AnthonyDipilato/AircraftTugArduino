/* Run Class
 *  
 *  Manages operations in loop
 *  
 */



#ifndef Running_h
#define Running_h

#include "Arduino.h"


class Running
{
  public:
    Running();
    void loop_();
    void setMotor(int motor, int percent);
    void toggleRelay(int relay, boolean state);
    void motorCheck(); // safety check to prevent runaway tug
    void hornCheck();
    void setMotorSpeed(int address, int value);
    void setMotorDirection(int address, int value); 
    void allStop();

    int motorLPercent;
    int motorLDirection;
    int motorRPercent;
    int motorRDirection;

    // booleans for relay states
    boolean hornState;
    boolean lightState;
    boolean strobeState;
    boolean hitchState;
    // motor values
    int motorLState;
    int motorRState;

    unsigned long lastUpdateL;
    unsigned long lastUpdateR;
    unsigned long hornStart;
    unsigned long currentTime;

    byte outputValue;
    
  private:
    int none;
    
};


#endif

