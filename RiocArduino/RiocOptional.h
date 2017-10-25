
// optional objects can be enabled or disabled 

#define OPT_ENABLE_THERMOMETER

#if not defined(__SAM3X8E__) 
  #define OPT_ENABLE_IR_TRANSMITTER	
  #define OPT_ENABLE_IR_RECEIVER
#endif

// use software tone playing 
#if defined(OPT_ENABLE_IR_TRANSMITTER) || defined(OPT_ENABLE_IR_RECEIVER) || defined(__SAM3X8E__) 
	#define OPT_SOFTWARE_TONE
#endif

