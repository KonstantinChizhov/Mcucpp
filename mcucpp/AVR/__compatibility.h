
// Compiler compatibility header

#if defined(__ICCAVR__) // IAR for AVR
	#define ENABLE_BIT_DEFINITIONS
	#include <ioavr.h>
#elif defined(__AVR__) // avr-gcc
	#include <avr/io.h>
#else
	#error "Unknown compiler"
#endif