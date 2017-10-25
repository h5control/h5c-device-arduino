#ifndef LIB_RIOC_MESSAGER_H_
#define LIB_RIOC_MESSAGER_H_

#include <Arduino.h>

class RiocMessager {
		
public:
  RiocMessager(byte address = 1);
  ~RiocMessager();
	
  virtual void begin();
  virtual void sendMessage(byte msg[8], byte address_to = 0);				
  virtual void process();

  // set callbacks on receiving messages
  void setCallbackMessageReceived(void (*fptr)(byte msg[8], byte address_from));
	
protected:

  void (*_callbackMessageReceived)(byte msg[8], byte address_from);
  
};

#endif // LIB_RIOC_MESSAGER_H_
