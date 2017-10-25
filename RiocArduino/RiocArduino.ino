/*

	RIOC for Arduino
	Copyright 2016 Robin Zhang
 	robin@atomixtech.com

	NOTE: 

	Arduino UNO, Nano or any others with ATmega328 MCU has only 32K bytes for program storage.
	You can edit file RiocOptional.h to disable some optional control objects when you need
	recompile the program for a smaller storage space occupied in Arduino.

	If your Arduino is a MEGA 1280 / 2560, please just keep the default settings to compile 
	the program with all optional control objects.

	This program has been tested with Arduino UNO, Duemilanove, Nano, MEGA and DUE, 
	other Arduino boards may not be supported.


	Change List

	v0.10  2016/08/18
	+ added synchronous messaging support
	+ added unit wait command
	+ added unit silent command

	v0.11  2016/08/20
	! updated tone player to remove the dependence with TIMER2
	+ added IR transmitter and receiver objects
	+ added thermometer object (dht11 / dht22)

*/

#include "RiocDevice.h"

void setup()
{   
  Serial.begin(115200);
  initRioc();
}

void loop()
{
  processRioc();
}

