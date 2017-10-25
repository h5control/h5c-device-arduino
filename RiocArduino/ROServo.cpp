#include "ROServo.h"
#include "RiocMessager.h"
#include <Servo.h>

ROServo::ROServo()
{
  _pin = -1;
  _mode = -1;
  _servo = NULL;
}


ROServo::~ROServo()
{
}

bool ROServo::setup(byte msg[8], byte address_from)
{
  if (_pin != -1) return false;

  int pin = msg[2];
  int mode = msg[3];

  if (pin<=DO_PIN_MAX_NUMBER) {

    _pin = pin;
    _mode = mode;

    _servo = new Servo();
    _servo->attach(_pin);

    return true;
  } 
  return false;
}

void ROServo::execute(byte msg[8], byte address_from)
{
  if (_pin == -1) return; 

  int cmd = msg[1];
  if (cmd==0x01) {
    
    // SET ANGLE
    
    int angle = msg[3];

    if (angle > 180) angle = 180;
    _servo->write(angle);

    if (!isSilent()) {
      byte rsp[] = {0x13, 0x81, _pin, angle, 0, 0, 0, 0};
      if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
    }

  } else if (cmd==0x02) {

    // GET ANGLE

    int angle = _servo->read();

    byte rsp[] = {0x13, 0x82, _pin, angle, 0, 0, 0, 0};
    if (_messager!=NULL) _messager->sendMessage(rsp, address_from);

  } else if (cmd==0x03) {

    // SET ENABLE

    bool enabled = msg[3];
    if (enabled) {
      if (!_servo->attached()) _servo->attach(_pin);
    } else {
      if (_servo->attached()) _servo->detach();
    }

    if (!isSilent()) {
      byte rsp[] = {0x13, 0x83, _pin, enabled, 0, 0, 0, 0};
      if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
    }
  
  } else if (cmd==0x04) {

    // GET ENABLE

    bool enabled = _servo->attached();

    byte rsp[] = {0x13, 0x84, _pin, enabled, 0, 0, 0, 0};
    if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
  }  

}

void ROServo::process()
{    
}
