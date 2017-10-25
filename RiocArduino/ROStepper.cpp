#include "ROStepper.h"
#include "RiocMessager.h"
#include <Stepper.h>

ROStepper::ROStepper()
{
  _pin1 = -1;
  _pin2 = -1;
  _pin3 = -1;
  _pin4 = -1;
  _mode = -1;

  _stepper = NULL;

  _speed = 100;
  _stepInterval = 1000000 / _speed;;
  _lastStepTime = micros();
  _positionCurrent = 0;
  _positionTarget = 0;
}


ROStepper::~ROStepper()
{
}

bool ROStepper::setup(byte msg[8], byte address_from)
{
  if (_pin1 != -1) return false;

  int pin1 = msg[2];
  int pin2 = msg[3];
  int pin3 = msg[4];
  int pin4 = msg[5];  
  int mode = msg[6];

  if (pin1<=DO_PIN_MAX_NUMBER && pin2<=DO_PIN_MAX_NUMBER && 
      pin3<=DO_PIN_MAX_NUMBER && pin4<=DO_PIN_MAX_NUMBER &&
      pin1!=pin2 && pin1!=pin3 && pin1!=pin4 && 
      pin2!=pin3 && pin2!=pin4 && pin3!=pin4) {

    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    _pin4 = pin4;    
    _mode = mode;

    _stepper = new Stepper(60, _pin1, _pin2, _pin3, _pin4); // need set steps per rotation as 60
    _stepper->setSpeed(_speed);

    return true;
  } 
  return false;
}

void ROStepper::execute(byte msg[8], byte address_from)
{
  if (_pin1 == -1) return; 

  int cmd = msg[1];
  if (cmd==0x01) {

    // STEP

    int dir = (msg[3] ? 1 : 0);
    unsigned int steps = ((unsigned int)msg[4] << 8) | (unsigned int)msg[5];

    if (steps>0x7fff) steps = 0x7fff;

    if (steps>0) {
      _stepper->step((dir ? -1: 1) * steps);
      _positionCurrent += (long)(dir ? -1: 1) * (long)steps;
      _positionCurrent = constrain(_positionCurrent, -0xffffff, 0xffffff);
      _positionTarget = _positionCurrent;
    }

    if (!isSilent()) {
      byte rsp[] = {0x12, 0x81, _pin1, dir, steps >> 8, steps & 0xff, 0, 0};
      if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
    }  
  
  } else if (cmd==0x02) {

    // GOTO

    _positionTarget = (msg[3] ? -1 : 1) * 
                      (((long)msg[4] << 16) | ((long)msg[5] << 8) | (long)msg[6]);

    if (!isSilent()) {
      byte sig = (_positionTarget>=0 ? 0 : 1);
      long val = (_positionTarget>=0 ? _positionTarget : -_positionTarget);
      byte rsp[] = {0x12, 0x82, _pin1, sig, val >> 16, val >> 8, val & 0xff, 0};
      if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
    }  
  
  } else if (cmd==0x03) {
      
    // STOP

    _positionTarget = _positionCurrent;

    if (!isSilent()) {
      byte rsp[] = {0x12, 0x83, _pin1, 0, 0, 0, 0, 0};
      if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
    }

  } else if (cmd==0x04) {
      
    // SET SPEED

    _speed = ((unsigned int)msg[3]<<8) | (unsigned int)msg[4];
    _stepInterval = 1000000 / _speed; // microseconds per step 
    _stepper->setSpeed(_speed);

    if (!isSilent()) {
      byte rsp[] = {0x12, 0x84, _pin1, _speed>>8, _speed & 0xff, 0, 0, 0};
      if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
    }

  } else if (cmd==0x05) {
      
    // GET SPEED

    byte rsp[] = {0x12, 0x85, _pin1, _speed>>8, _speed & 0xff, 0, 0, 0};
    if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
  
  } else if (cmd==0x06) {
    
    // SET POSITION

    byte sig = (msg[3] ? 1 : 0);
    long val = ((long)msg[4]<<16) | ((long)msg[5]<<8) | (long)msg[6];
    _positionCurrent = (sig ? -1 : 1)*val;
    
    if (!isSilent()) {
      byte rsp[] = {0x12, 0x86, _pin1, sig, val>>16, val>>8, val & 0xff, 0};
      if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
    }

  } else if (cmd==0x07) {
      
    // GET POSITION

    byte sig = (_positionCurrent>=0 ? 0 : 1);
    long val = (_positionCurrent>=0 ? _positionCurrent : -_positionCurrent);

    byte rsp[] = {0x12, 0x87, _pin1, sig, val>>16, val>>8, val & 0xff, 0};
    if (_messager!=NULL) _messager->sendMessage(rsp, address_from);
  }
}

void ROStepper::process()
{
  if (_pin1 == -1) return; 

  // step
  unsigned long thisTime = micros();
  if (_positionCurrent != _positionTarget && ( thisTime - _lastStepTime > _stepInterval || thisTime < _lastStepTime )) {
    
    int val = (_positionCurrent > _positionTarget ? -1 : 1);
    _stepper->step(val);
    _positionCurrent += val;
    _positionCurrent = constrain(_positionCurrent, -0xffffff, 0xffffff);
    _lastStepTime = thisTime;
  }
}
