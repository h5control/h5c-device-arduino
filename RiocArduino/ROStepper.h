#ifndef LIB_RO_STEPPER_H_
#define LIB_RO_STEPPER_H_

#include "RiocObject.h"

class Stepper;

class ROStepper : public RiocObject {

public:
  ROStepper();
  ~ROStepper();

  bool setup(byte msg[8], byte address_from = 0);
  void execute(byte msg[8], byte address_from = 0);
  void process();

private:  
  int _pin1;
  int _pin2;
  int _pin3;
  int _pin4;  
  int _mode;

  Stepper* _stepper;

  unsigned int _speed;
  unsigned int _stepInterval;
  unsigned long _lastStepTime;
  long _positionCurrent;
  long _positionTarget;

};

#endif // LIB_RO_STEPPER_H_
