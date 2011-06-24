

// list of used IO ports
#define USE_PORTA
#define USE_PORTB
#define USE_PORTC

// NOTE that you need to add library platform independent and platform specific
// folders to compiler include files search paths:
// '../../mcucpp' and '../../mcucpp/ARM/STM32' respectively for this example

// defination of TPin class
#include <iopins.h>

// defination of PinList class
#include <pinlist.h>

// all IO stuff are in namespce
using namespace IO;
// working with ports

void Ports()
{
	// set all pins in port to output
	Porta::SetConfiguration(0xff, Porta::Out); 
	// write a value to port
	Porta::Write(0x55);	
	// set pins with mask
	Porta::Set(0xAA); 	
	// clear pins with mask
	Porta::Clear(0xF0); 
	// togle pins with mask
	Porta::Toggle(0xFF);

	uint8_t clearMask = 0x0F;
	uint8_t outputValue = 0x03;
	Porta::ClearAndSet(clearMask, outputValue);
	// read out register

	// set all pins in port to input
	Porta::SetConfiguration(0xff, Porta::In);
	uint8_t value = Porta::Read(); 
	// read input register
	value = Porta::PinRead(); 

	// template versions of port manipulations for writing constant values
	// these functions are guaranteed to be inlined for maximum speed

	Porta::SetConfiguration<0xff, Porta::Out>();
	Porta::Set<0xAA>();
	Porta::Clear<0xF0>();
	Porta::Toggle<0xFF>();
	const uint8_t clearMask2 = 0x0F;
	const uint8_t outputValue2 = 0x03;
	Porta::ClearAndSet<clearMask2, outputValue2>();
}

// working with individual pins
void IndividualPins()
{
	// definition of one IO pin: pin 1 at port
	typedef TPin<Porta, 1> Pin1;
	// or you can use predefined short name
	// typedef Pa1 Pin1;

	// Configure pin as output
	Pin1::SetConfiguration(Pin1::Port::Out); 
	// set pin to logical 1
	Pin1::Set();
	// set pin to logical 0
	Pin1::Clear();
	// toggle pin state
	Pin1::Toggle();
	// Configure pin as input
	Pin1::SetConfiguration(Pin1::Port::In);
	// check pin state
	if(Pin1::IsSet()) 
	{
		// do something
	}


	// definition of one IO pin: pin 1 at PORTA with inverted logic
	// ie. 'Set' will write logical 0 to pin and 'Clear' - logical 1.
	// reading (IsSet) is not inverted
	typedef InvertedPin<Porta, 1> Pin2;
	// or you can use predefined short name
	// typedef Pa1Inv Pin2;

	// Configure pin as output
	Pin2::SetConfiguration(Pin2::Port::Out);
	// set pin to logical 1
	Pin2::Set();
	// set pin to logical 0
	Pin2::Clear();
	// toggle pin state
	Pin2::Toggle();
	// Configure pin as input
	Pin2::SetConfiguration(Pin2::Port::In);
	// check pin state
	if(Pin2::IsSet()) 
	{
		// do something
	}
}

// wotking with groups of pins - pin lists
void PinLists()
{
	// Definition of group of IO pins which acts like a virtual IO port.
	// One group can contain up to 32 pins from different ports.
	// Pins in the group can have an arbitrary order.

	typedef PinList<Pa1, Pb0, Pa2, Pb1, Pc3, Pc4, Pc5> Group1;
	
	// You can include inverted pins to the group
	typedef PinList<Pa1, Pb0Inv, Pa2, Pb1, Pc3Inv, Pc4, Pc5> Group2;
	
	// PinList has interface similar to IO port interface,
	// but is is a little simplified

	Group1::SetConfiguration(Group1::Out);
	// write a value to group
	Group1::Write(0x55);
	// set pins with mask
	Group1::Set(0xAA); 
	// clear pins with mask
	Group1::Clear(0xF0);

	// set all pins in group to input
	Group1::SetConfiguration(Group1::In);
	// read input register
	uint8_t value = Group1::PinRead(); 

	// If you have a constant value to write to group,
	// use template varsions of writing functions.
	// They are much faster and smaller, since most of things are evaluated
	// at compile time and only actual Read/Modify/Write operation will take place in runtime.

	Group1::SetConfiguration<Group1::Out, 0xff>();
	Group1::Write<0x55>();
	Group1::Set<0xAA>();
	Group1::Clear<0xF0>();

	// Individual pins in group can be accessed in this way:
	// Set pin 1 in group (indexing starts whith 0)
	Group1::Pin<1>::Set();
	// you can 'typdef' it for best readability
	typedef Group1::Pin<1> Pin1;
	Pin1::Clear();
	// Toggle the last pin in the group
	const int lastPinIndex = Group1::Length - 1;
	Group1::Pin<lastPinIndex>::Toggle();

	// You can take a slice of group, it will be another PinList
	const int startingIndex = 4;
	const int sliceLenth = 3;
	typedef Group1::Slice<startingIndex, sliceLenth> LastTreePins;
	// Note that sliced pins will have they origin bit position in the input value.
	// ie. other pins in group will be just masked out
	LastTreePins::Write(0x70);
}


int main()
{
  // enable port clocking
	Porta::Enable();
 	Portb::Enable();
    Portc::Enable();
	
	Ports();
	IndividualPins();
	PinLists();
}
