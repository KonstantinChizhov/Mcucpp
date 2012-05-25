#if defined(__AT90S4433__) || defined(__AVR_AT90S4433__)
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega649P__) || defined(__AVR_ATmega649P__) // atmega649p
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT90S4434__) || defined(__AVR_AT90S4434__) // at90s4434
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega168PA__) // atmega168pa
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__AVR_ATmega649A__) // atmega649a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AVR_AT76C711__) // at76c711
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT76C713__) // at76c713
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT76C712__) // at76c712
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega649__) || defined(__AVR_ATmega649__) // atmega649
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega168P__) || defined(__AVR_ATmega168P__) // atmega168p
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATtiny40__) // attiny40
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega128A__) // atmega128a
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega640__) || defined(__AVR_ATmega640__) // atmega640
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega644__) || defined(__AVR_ATmega644__) // atmega644
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATtiny48__) || defined(__AVR_ATtiny48__) // attiny48
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega3250__) || defined(__AVR_ATmega3250__) // atmega3250
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT90PWM3B__) || defined(__AVR_AT90PWM3B__) // at90pwm3b
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega6490__) || defined(__AVR_ATmega6490__) // atmega6490
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega8HVA__) || defined(__AVR_ATmega8HVA__) // atmega8hva
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega64M1__) || defined(__AVR_ATmega64M1__) // atmega64m1
	typedef Pd0 SsPin;
	typedef Pb1 MosiPin;
	typedef Pb0 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega16__) || defined(__AVR_ATmega16__) // atmega16
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__AT90USB162__) || defined(__AVR_AT90USB162__) // at90usb162
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega64C1__) || defined(__AVR_ATmega64C1__) // atmega64c1
	typedef Pd0 SsPin;
	typedef Pb1 MosiPin;
	typedef Pb0 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__AT90PWM2B__) || defined(__AVR_AT90PWM2B__) // at90pwm2b
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega1284__) // atmega1284
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega64__) || defined(__AVR_ATmega64__) // atmega64
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega1280__) || defined(__AVR_ATmega1280__) // atmega1280
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega1281__) || defined(__AVR_ATmega1281__) // atmega1281
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT90PWM316__) || defined(__AVR_AT90PWM316__) // at90pwm316
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega64A__) // atmega64a
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT43USB355__) || defined(__AVR_AT43USB355__) // at43usb355
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT43USB351M__) // at43usb351m
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATtiny167__) || defined(__AVR_ATtiny167__) // attiny167
	typedef Pa6 SsPin;
	typedef Pa4 MosiPin;
	typedef Pa2 MisoPin;
	typedef Pa5 SckPin;

#elif defined(__AT90S2333__) || defined(__AVR_AT90S2333__) // at90s2333
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega328P__) || defined(__AVR_ATmega328P__) // atmega328p
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega3290PA__) // atmega3290pa
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega16A__) || defined(__AVR_ATmega16A__) // atmega16a
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega32C1__) || defined(__AVR_ATmega32C1__) // atmega32c1
	typedef Pd0 SsPin;
	typedef Pb1 MosiPin;
	typedef Pb0 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega16HVA2__) || defined(__AVR_ATmega16HVA2__) // atmega16hva2
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT90CAN32__) || defined(__AVR_AT90CAN32__) // at90can32
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega128RFA1__) || defined(__AVR_ATmega128RFA1__) // atmega128rfa1
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega8A__) // atmega8a
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega8__) || defined(__AVR_ATmega8__) // atmega8
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega161__) || defined(__AVR_ATmega161__) // atmega161
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__AVR_ATmega6490A__) // atmega6490a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATtiny20__) // attiny20
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AVR_AT43USB320__) // at43usb320
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega103__) || defined(__AVR_ATmega103__) // atmega103
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega64HVE__) || defined(__AVR_ATmega64HVE__) // atmega64hve
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega88PA__) || defined(__AVR_ATmega88PA__) // atmega88pa
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega169__) || defined(__AVR_ATmega169__) // atmega169
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega168__) || defined(__AVR_ATmega168__) // atmega168
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATtiny87__) || defined(__AVR_ATtiny87__) // attiny87
	typedef Pa6 SsPin;
	typedef Pa4 MosiPin;
	typedef Pa2 MisoPin;
	typedef Pa5 SckPin;

#elif defined(__ATmega324PA__) // atmega324pa
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATtiny88__) || defined(__AVR_ATtiny88__) // attiny88
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega163__) || defined(__AVR_ATmega163__) // atmega163
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega162__) || defined(__AVR_ATmega162__) // atmega162
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega165__) || defined(__AVR_ATmega165__) // atmega165
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega164__) // atmega164
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega16HVA__) || defined(__AVR_ATmega16HVA__) // atmega16hva
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega16HVB__) || defined(__AVR_ATmega16HVB__) // atmega16hvb
	typedef Pb4 SsPin;
	typedef Pb6 MosiPin;
	typedef Pb7 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATA6286__) // ata6286
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT90PWM2__) || defined(__AVR_AT90PWM2__) // at90pwm2
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT90PWM3__) || defined(__AVR_AT90PWM3__) // at90pwm3
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATA6289__) || defined(__AVR_ATA6289__) // ata6289
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega164PA__) // atmega164pa
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega169P__) || defined(__AVR_ATmega169P__) // atmega169p
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega2560__) || defined(__AVR_ATmega2560__) // atmega2560
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega2561__) || defined(__AVR_ATmega2561__) // atmega2561
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega164A__) || defined(__AVR_ATmega164A__) // atmega164a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega3290P__) || defined(__AVR_ATmega3290P__) // atmega3290p
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega48PA__) // atmega48pa
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__AVR_ATmega169A__) // atmega169a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega16U4__) || defined(__AVR_ATmega16U4__) // atmega16u4
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega16U2__) || defined(__AVR_ATmega16U2__) // atmega16u2
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega3250PA__) // atmega3250pa
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT90PWM1__) || defined(__AVR_AT90PWM1__) // at90pwm1
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATA6285__) // ata6285
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AVR_ATmega645A__) // atmega645a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega645__) || defined(__AVR_ATmega645__) // atmega645
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega1284P__) || defined(__AVR_ATmega1284P__) // atmega1284p
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__AVR_ATmega645P__) // atmega645p
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega3290__) || defined(__AVR_ATmega3290__) // atmega3290
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega8U2__) || defined(__AVR_ATmega8U2__) // atmega8u2
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega16M1__) || defined(__AVR_ATmega16M1__) // atmega16m1
	typedef Pd0 SsPin;
	typedef Pb1 MosiPin;
	typedef Pb0 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__AT90CAN128__) || defined(__AVR_AT90CAN128__) // at90can128
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega325P__) || defined(__AVR_ATmega325P__) // atmega325p
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega164P__) || defined(__AVR_ATmega164P__) // atmega164p
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega644PA__) || defined(__AVR_ATmega644PA__) // atmega644pa
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega8515__) || defined(__AVR_ATmega8515__) // atmega8515
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega329PA__) || defined(__AVR_ATmega329PA__) // atmega329pa
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega88A__) || defined(__AVR_ATmega88A__) // atmega88a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT90PWM81__) || defined(__AVR_AT90PWM81__) // at90pwm81
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT90USB1286__) || defined(__AVR_AT90USB1286__) // at90usb1286
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT90USB1287__) || defined(__AVR_AT90USB1287__) // at90usb1287
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega6450__) || defined(__AVR_ATmega6450__) // atmega6450
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega329P__) || defined(__AVR_ATmega329P__) // atmega329p
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT90S8515__) || defined(__AVR_AT90S8515__) // at90s8515
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__AT43USB320A__) // at43usb320a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT90CAN64__) || defined(__AVR_AT90CAN64__) // at90can64
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega32HVB__) || defined(__AVR_ATmega32HVB__) // atmega32hvb
	typedef Pb4 SsPin;
	typedef Pb6 MosiPin;
	typedef Pb7 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__AVR_ATmega6450P__) // atmega6450p
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega32U2__) || defined(__AVR_ATmega32U2__) // atmega32u2
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega32U6__) || defined(__AVR_ATmega32U6__) // atmega32u6
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega32U4__) || defined(__AVR_ATmega32U4__) // atmega32u4
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT90PWM216__) || defined(__AVR_AT90PWM216__) // at90pwm216
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AVR_ATmega6450A__) // atmega6450a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AT90S8535__) || defined(__AVR_AT90S8535__) // at90s8535
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega48__) || defined(__AVR_ATmega48__) // atmega48
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__AT90S4414__) || defined(__AVR_AT90S4414__) // at90s4414
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega48A__) || defined(__AVR_ATmega48A__) // atmega48a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega48P__) || defined(__AVR_ATmega48P__) // atmega48p
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega88__) || defined(__AVR_ATmega88__) // atmega88
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__AT90USB82__) || defined(__AVR_AT90USB82__) // at90usb82
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega32M1__) || defined(__AVR_ATmega32M1__) // atmega32m1
	typedef Pd0 SsPin;
	typedef Pb1 MosiPin;
	typedef Pb0 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega324A__) || defined(__AVR_ATmega324A__) // atmega324a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega8535__) || defined(__AVR_ATmega8535__) // atmega8535
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega32A__) // atmega32a
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__AT90USB646__) || defined(__AVR_AT90USB646__) // at90usb646
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT90USB647__) || defined(__AVR_AT90USB647__) // at90usb647
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AT90SCR100__) || defined(__AVR_AT90SCR100__) // at90scr100
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega324P__) || defined(__AVR_ATmega324P__) // atmega324p
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega325PA__) // atmega325pa
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega644A__) || defined(__AVR_ATmega644A__) // atmega644a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega329__) || defined(__AVR_ATmega329__) // atmega329
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega328__) || defined(__AVR_ATmega328__) // atmega328
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega325__) || defined(__AVR_ATmega325__) // atmega325
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega324__) // atmega324
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega323__) || defined(__AVR_ATmega323__) // atmega323
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega128__) || defined(__AVR_ATmega128__) // atmega128
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__AVR_ATmega6490P__) // atmega6490p
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega644P__) || defined(__AVR_ATmega644P__) // atmega644p
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega165PA__) // atmega165pa
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega32__) || defined(__AVR_ATmega32__) // atmega32
	typedef Pb4 SsPin;
	typedef Pb5 MosiPin;
	typedef Pb6 MisoPin;
	typedef Pb7 SckPin;

#elif defined(__ATmega168A__) || defined(__AVR_ATmega168A__) // atmega168a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__AVR_ATmega165A__) // atmega165a
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega169PA__) || defined(__AVR_ATmega169PA__) // atmega169pa
#	error "TODO: define SPI pins for this device"
//	typedef Pb4 SsPin;
//	typedef PB5 MosiPin;
//	typedef Pb6 MisoPin;
//	typedef PB7 SckPin;

#elif defined(__ATmega88P__) || defined(__AVR_ATmega88P__) // atmega88p
	typedef Pb2 SsPin;
	typedef Pb3 MosiPin;
	typedef Pb4 MisoPin;
	typedef Pb5 SckPin;

#elif defined(__ATmega165P__) || defined(__AVR_ATmega165P__) // atmega165p
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#elif defined(__ATmega3250P__) || defined(__AVR_ATmega3250P__) // atmega3250p
	typedef Pb0 SsPin;
	typedef Pb2 MosiPin;
	typedef Pb3 MisoPin;
	typedef Pb1 SckPin;

#else
#	error "This module is not supported in selected device"
#endif
