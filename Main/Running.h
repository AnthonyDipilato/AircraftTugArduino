/* Run Class
 *  
 *  Manages operations in loop
 *  
 */

#ifndef Run_h
#define Run_h

#include "Arduino.h"

class Run
{
  public:
    Run(int test);
    void loop_();
    void setMotor(int motor, int percent);
    void toggleRelay(int relay, boolean state);
    void motorCheck(); // safety check to prevent runaway tug

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

    unsigned long lastCheck;
    unsigned long lastUpdateL;
    unsigned long lastUpdateR;
    
  private:
    int none;
    
};


#endif

