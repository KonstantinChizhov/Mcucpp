//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#include <iopins.h>
#include <pinlist.h>
namespace Mcucpp
{

	template<class Mosi, class Miso, class Clock>
	class SoftSpi
	{
		typedef IO::PinList<Mosi, Clock> OutPins;

		public:
		static uint8_t ReadWrite(uint8_t value)
		{
			//OutPins::SetConfiguration(OutPins::Out, 0xff);
			Clock::SetConfiguration(Clock::Port::Out);
			Mosi::SetConfiguration(Mosi::Port::Out);
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
}
