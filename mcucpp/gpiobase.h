
#pragma once

#ifndef IOPORTBASE_HPP
#define IOPORTBASE_HPP

namespace IO
{

	template<unsigned long x>
	class PopulatedBits
	{
		static const unsigned long x1 = (x & 0x55555555) + ((x >> 1) & 0x55555555);
		static const unsigned long x2 = (x1 & 0x33333333) + ((x1 >> 2) & 0x33333333);
		static const unsigned long x3 = (x2 & 0x0f0f0f0f) + ((x2 >> 4) & 0x0f0f0f0f);
		static const unsigned long x4 = (x3 & 0x00ff00ff) + ((x3 >> 8) & 0x00ff00ff);
	public:
		static const unsigned long value = (x4 & 0x0000ffff) + ((x4 >> 16) & 0x0000ffff);
	};


	class GpioBase
	{
		GpioBase();
	public:
		enum GenericConfiguration
		{
			AnalogIn 		= 0,
			In 				= 1,
			PullUpOrDownIn 	= 2,

			Out 			= 4,
			OpenDrainOut 	= 5,
			AltOut 			= 6,
			AltOpenDrain 	= 7
		};

		// define 'Configuration' to this type to indicate that derived port class does not need any configuration
		enum DontCareConfiguration{None};
	};	
}
#endif
