/*    Buttons Class
 *   
 *    Manager class to clean up the main file and handle status and debounce of buttons.
 *  
 */

#ifndef Buttons_h
#define Buttons_h

#include "Arduino.h"

class Buttons
{
    public:
        Buttons(int pin);
        void check();
        int value;
    private:
        int _pin;
        long debounce;
        int last_status;
        int debounce_delay;
};



#endif

