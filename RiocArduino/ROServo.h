#ifndef LIB_RO_SERVO_H_
#define LIB_RO_SERVO_H_

#include "RiocObject.h"

class Servo;

class ROServo : public RiocObject {

public:
  ROServo();
  ~ROServo();

  bool setup(byte msg[8], byte address_from = 0);
  void execute(byte msg[8], byte address_from = 0);
  void process();

private:  
  int _pin;
  int _mode;

  Servo* _servo;
};

#endif // LIB_RO_SERVO_H_
