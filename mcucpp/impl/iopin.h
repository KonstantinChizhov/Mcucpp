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

namespace Mcucpp
{
	namespace IO
	{
		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::Set()
		{
			Set(true);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::Set(bool val)
		{
			if(val)
				PORT::template Set<1u << PIN>();
			else
				PORT::template Clear<1u << PIN>();
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetDir(uint8_t val)
		{
			if(val)
				SetDirWrite();
			else SetDirRead();
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::Clear()
		{
			Set(false);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::Toggle()
		{
			PORT::Toggle(1u << PIN);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetDirRead()
		{
			PORT::template SetPinConfiguration<PIN>( PORT::In);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetDirWrite()
		{
			ConfigPort:: template SetPinConfiguration<PIN>(PORT::Out);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetConfiguration(Configuration configuration)
		{
			ConfigPort:: template SetPinConfiguration<PIN>(configuration);
		}
		
		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetDriverType(DriverType driverType)
		{
			ConfigPort::SetDriverType(1u << PIN, driverType);
		}
		
		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetPullUp(PullMode pullMode)
		{
			ConfigPort::SetPullUp(1u << PIN, pullMode);
		}
		
		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetSpeed(Speed speed)
		{
			ConfigPort::SetSpeed(1u << PIN, speed);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::AltFuncNumber(uint8_t funcNumber)
		{
			ConfigPort::AltFuncNumber(1u << PIN, funcNumber);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		bool TPin<PORT, PIN, CONFIG_PORT>::IsSet()
		{
			return (PORT::PinRead() & (typename PORT::DataT)(1u << PIN)) != 0;
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::WaiteForSet()
		{
			while(IsSet()==0){}
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::WaiteForClear()
		{
			while(IsSet()){}
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void InvertedPin<PORT, PIN, CONFIG_PORT>::Set(bool val)
		{
			if(val)
				PORT::template Clear<1u << PIN>();
			else
				PORT::template Set<1u << PIN>();
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void InvertedPin<PORT, PIN, CONFIG_PORT>::Set()
		{
			Set(true);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void InvertedPin<PORT, PIN, CONFIG_PORT>::Clear()
		{
			Set(false);
		}
		
	}
}
