
#include <iopins.h>
#include <pinlist.h>

template<class Mosi, class Miso, class Clock>
class SoftSpi
{
	typedef IO::PinList<Mosi, Miso, Clock> OutPins;
	
	public:
	static uint8_t ReadWrite(uint8_t value)
	{
		OutPins::SetConfiguration(OutPins::Out, 0xff);
		Miso::SetConfiguration(Miso::Port::In);
		
		for(uint8_t i = 0; i < 8;i++)
		{
			Mosi::Set(value & 0x80);
			Clock::Set();
			value <<= 1;
			if(Miso::IsSet())
				value |= 1;
			Clock::Clear();
		}
		return value;
	}
};
