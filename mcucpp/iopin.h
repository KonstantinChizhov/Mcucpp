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


#pragma once

#include "static_assert.h"
#include <stdint.h>

namespace Mcucpp
{
namespace IO
{
	// this class represents one pin in a IO port.
	// It is fully static.
	template<class PORT, uint8_t PIN, class CONFIG_PORT = PORT>
	class TPin
	{
		STATIC_ASSERT(PIN < PORT::Width);
	public:
		typedef PORT Port;
		typedef CONFIG_PORT ConfigPort;
		
		typedef typename ConfigPort::Speed Speed;
		typedef typename ConfigPort::PullMode PullMode;
		typedef typename ConfigPort::DriverType DriverType;
		typedef typename ConfigPort::Configuration Configuration;

		static const unsigned Number = PIN;
		static const bool Inverted = false;

		static void Set();
		static void Set(bool val);
		static void SetDir(uint8_t val);

		static void Clear();
		static void Toggle();
		static void SetDirRead();
		static void SetDirWrite();
		static void SetConfiguration(Configuration configuration);
		
		static void SetDriverType(DriverType driverType);
		static void SetPullUp(PullMode pullMode);
		static void SetSpeed(Speed speed);
		static void AltFuncNumber(uint8_t funcNumber);
	
		template<Configuration configuration>
		static void SetConfiguration()
		{
			ConfigPort:: template SetConfiguration<1 << PIN, configuration>();
		}

		static bool IsSet();
		static void WaiteForSet();
		static void WaiteForClear();
	};

	template<class PORT, uint8_t PIN, class CONFIG_PORT = PORT>
	class InvertedPin :public TPin<PORT, PIN, CONFIG_PORT>
	{
	public:
		static const bool Inverted = true;

		static void Set(bool val);
		static void Set();
		static void Clear();
	};

}
}
#include <impl/iopin.h>
