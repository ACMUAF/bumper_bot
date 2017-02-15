#ifndef __BUMPERBOT_STUFF
#define __BUMPERBOT_STUFF

#include "Arduino.h"


class ToggleP{
  public:
    ToggleP(int p):_p(p),_s(false){}
    ToggleP(int p, bool s):_p(p),_s(s){}

    bool tog(){
      _s=!_s;
      digitalWrite(_p,_s);//High and low are the same as True and False
      return _s;
    }

    bool set(bool s){
      _s=s;
      digitalWrite(_p,_s);
      return _s;
    }

    bool state(){return _s;}

  private:
    int  _p;  //pin number
    bool _s;  //current state  
};

#endif
