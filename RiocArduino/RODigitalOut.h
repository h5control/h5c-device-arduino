#ifndef LIB_RO_DIGITAL_OUT_H_
#define LIB_RO_DIGITAL_OUT_H_

#include "RiocObject.h"

class RODigitalOut : public RiocObject {

public:
  RODigitalOut();
  ~RODigitalOut();

  bool setup(byte msg[8], byte address_from = 0);
  void execute(byte msg[8], byte address_from = 0);
  void process();

private:  
  int _pin;
  int _mode;

  int _val;
  int _pwm;
};


#endif // LIB_RO_DIGITAL_OUT_H_
