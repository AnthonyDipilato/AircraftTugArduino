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
    void setTargetSpeed(int address, int value);
    void toggleRelay(int relay, boolean state);
    void motorCheck(); // safety check to prevent runaway tug
    void hornCheck();
    void setMotorSpeed(int address, int value); 
    void allStop();
    byte getOutputValue(int value);
    void motorLoop();

    int currentSpeedL;
    int currentSpeedR;
    int targetSpeedL;
    int targetSpeedR;

    // booleans for relay states
    boolean hornState;
    boolean lightState;
    boolean strobeState;
    boolean hitchState;

    unsigned long lastCommandL;
    unsigned long lastCommandR;
    unsigned long hornStart;
    unsigned long currentTime;
    unsigned long motorUpdate;

    byte outputValue;
    
    
  private:
    int none;
    
};


#endif

