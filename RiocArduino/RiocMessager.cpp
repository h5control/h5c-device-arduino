#include "RiocMessager.h"


RiocMessager::RiocMessager(byte address)
{
  // initialise callbacks to NULL pointer
  _callbackMessageReceived = NULL;
}


RiocMessager::~RiocMessager()
{

}

void RiocMessager::begin()
{

}

void RiocMessager::sendMessage(byte msg[8], byte address_to)
{

}

void RiocMessager::process()
{

}

void RiocMessager::setCallbackMessageReceived(void (*fptr)(byte msg[8], byte address_from))
{
  _callbackMessageReceived = fptr;
}
