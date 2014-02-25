
// NOTE that you need to add library platform independent and platform specific
// folders to compiler include files search Pbths:
// '../../mcucpp' and '../../mcucpp/AVR' respectively for this example

// defination of TPin class
#include <iopins.h>

// defination of PinList class
#include <pinlist.h>

// virtual port
#include <vport.h>

// all IO stuff are in namespce
using namespace Mcucpp::IO;
// working with ports

#if !defined(MCUCPP_HAS_PORTC)
	typedef NullPort Portc;
	typedef NullPin Pc0;
	typedef NullPin Pc1;
	typedef NullPin Pc2;
	typedef NullPin Pc3;
	typedef NullPin Pc4;
	typedef NullPin Pc5;
	
	typedef NullPin Pc0Inv;
	typedef NullPin Pc1Inv;
	typedef NullPin Pc2Inv;
	typedef NullPin Pc3Inv;
	typedef NullPin Pc4Inv;
	typedef NullPin Pc5Inv;
#endif

void Ports()
{
	Portb::Enable();
	// set all pins in Portb to output
	Portb::SetConfiguration(0xff, Portb::Out); // -> DDRA = 0xff;
	// write a value to port
	Portb::Write(0x55);	// -> Portb = 0x55;	
	// set pins with mask
	Portb::Set(0xAA); 	// -> Portb |= 0xAA;
	// clear pins with mask
	Portb::Clear(0xF0); // -> Portb &= ~0xf0;
	// togle pins with mask
	Portb::Toggle(0xFF);// -> Portb ^= ~0xf0;

	uint8_t clearMask = 0x0F;
	uint8_t outputValue = 0x03;
	Portb::ClearAndSet(clearMask, outputValue);// -> Portb = (Portb & ~clearMask) | outputValue;
	// read out register

	// set all pins in Portb to input
	Portb::SetConfiguration(0xff, Portb::In);
	uint8_t value = Portb::Read(); // -> 	uint8_t value = Portb
	(void)value;
	// read input register
	value = Portb::PinRead(); // -> value = PINA;

	// template versions of port manipulations for writing constant values
	// these functions are guaranteed to be inlined for maximum speed

	Portb::SetConfiguration<0xff, Portb::Out>();
	Portb::Set<0xAA>();
	Portb::Clear<0xF0>();
	Portb::Toggle<0xFF>();
	const uint8_t clearMask2 = 0x0F;
	const uint8_t outputValue2 = 0x03;
	Portb::ClearAndSet<clearMask2, outputValue2>();
}

// working with individual pins
void IndividualPins()
{
	Portb::Enable();
	// definition of one IO pin: pin 1 at Portb
	typedef TPin<Portb, 1> Pin1;
	// or you can use predefined short name
	// typedef Pb1 Pin1;

	// Configure pin as output
	Pin1::SetConfiguration(Pin1::Port::Out); // DDRA |= (1 << PinNumber);
	// set pin to logical 1
	Pin1::Set();
	// set pin to logical 0
	Pin1::Clear();
	// toggle pin state
	Pin1::Toggle();
	// Configure pin as input
	Pin1::SetConfiguration(Pin1::Port::In);
	// check pin state
	if(Pin1::IsSet()) // ->> if(PINA & (1 << PinNumber))
	{
		// do something
	}


	// definition of one IO pin: pin 1 at Portb with inverted logic
	// ie. 'Set' will write logical 0 to pin and 'Clear' - logical 1.
	// reading (IsSet) is not inverted
	typedef InvertedPin<Portb, 1> Pin2;
	// or you can use predefined short name
	// typedef Pb1Inv Pin2;

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
	if(Pin2::IsSet()) // ->> if(PINA & (1 << PinNumber))
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

	typedef PinList<Pb1, Pb0, Pb2, Pb1, Pc3, Pc4, Pc5> Group1;
	
	// You can include inverted pins to the group
	typedef PinList<Pb1, Pb0Inv, Pb2, Pb1, Pc3Inv, Pc4, Pc5> Group2;
	
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
	(void)value;
	// If you have a constant value to write to group,
	// use template varsions of writing functions.
	// They are much faster and smaller, since most of things are evaluated
	// at compile time and only actual Read/Modify/Write operation will take place in runtime.

	Group1::SetConfiguration<Group1::Out>();
	Group1::Write<0x55>();
	Group1::Set<0xAA>();
	Group1::Clear<0xF0>();

	// Individual pins in group can be accessed in this way:
	// Set pin 1 in group (indexing starts whith 0)
	Group1::Pin<1>::Set();
	// you can use 'typdef' for best readability
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

// working with dynamic virtual ports and pins 

void TestVPin(VPin &pin)
{
	pin.SetConfiguration(pin.Port.Out);
	pin.Set();
	pin.Clear();

}


void TestVPort(VPortBase & port)
{
	port.Write(0xff);
	port.Clear(0xff);

	VPin pin1(port, 1);
	TestVPin(pin1);
}


void VPorts()
{
	VPort<Portb> portb;
	TestVPort(portb);

	typedef PinList<Pb1, Pb0, Pb2, Pb1, Pc3, Pc4, Pc5> Group1;
	VPort<Group1> group1;
	TestVPort(group1);
}


int main()
{
	Ports();
	IndividualPins();
	PinLists();
}

extern "C" void __cxa_pure_virtual()
{
	while(1);
}

