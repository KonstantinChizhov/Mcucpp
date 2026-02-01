//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2025
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

#include <stdint.h>
#include <ports.h>

namespace Mcucpp
{

	using pin_id_t = uint8_t;

	class Pin
	{
		pin_id_t _id;
		GpioPort::DataT _mask;

	public:
		explicit Pin(pin_id_t id)
			: _id(id)
		{
			_mask = 1 << Number();
		}

		unsigned Number() { return _id & 0x0f; }

		GpioPort Port() const { return GpioPort(_id >> 4); }

		void Set() { Port().Set(_mask); }
		void Set(bool val)
		{
			if (val)
				Port().Set(_mask);
			else
				Port().Clear(_mask);
		}

		void Clear() { Port().Clear(_mask); }
		void Toggle() { Port().Toggle(_mask); }
		void SetConfiguration(PortCfg configuration) { Port().SetConfiguration(_mask, configuration); }

		void SetDriverType(DriverType driverType) { Port().SetDriverType(_mask, driverType); }
		void SetPullUp(PullMode pullMode) { Port().SetPullUp(_mask, pullMode); }
		void SetSpeed(PortSpeed speed) { Port().SetSpeed(_mask, speed); }
		void AltFuncNumber(uint8_t funcNumber) { Port().AltFuncNumber(_mask, funcNumber); }

		bool IsSet() { return (Port().PinRead() & _mask) != 0; }
	};

	template <class PORT, pin_id_t PIN>
	class StaticPin
	{
		static_assert(PIN < PORT::Width);

	public:
		typedef PORT Port;

		static constexpr unsigned Number = PIN;
		static constexpr bool Inverted = false;
		static constexpr Port::DataT Mask = 1 << Number;
		static constexpr uint8_t Id = (Port::Id << 4) | Number;

		static void Set() { Port::Set(Mask); }
		static void Set(bool val)
		{
			if (val)
				Port::Set(Mask);
			else
				Port::Clear(Mask);
		}

		static void Clear() { Port::Clear(Mask); }
		static void Toggle() { Port::Toggle(Mask); }
		static void SetConfiguration(PortCfg configuration) { Port::SetConfiguration(Mask, configuration); }

		static void SetDriverType(DriverType driverType) { Port::SetDriverType(Mask, driverType); }
		static void SetPullUp(PullMode pullMode) { Port::SetPullUp(Mask, pullMode); }
		static void SetSpeed(PortSpeed speed) { Port::SetSpeed(Mask, speed); }
		static void AltFuncNumber(uint8_t funcNumber) { Port::AltFuncNumber(Mask, funcNumber); }

		static bool IsSet() { return (Port::PinRead() & Mask) != 0; }
	};

	template <class PORT, pin_id_t PIN>
	class InvertedPin : public StaticPin<PORT, PIN>
	{
	public:
		static const bool Inverted = true;
		using StaticPin<PORT, PIN>::Mask;

		static void Set(bool val)
		{
			if (val)
				PORT::Clear(Mask);
			else
				PORT::Set(Mask);
		}
		static void Set() { PORT::Clear(Mask); }
		static void Clear() { PORT::Set(Mask); }
	};
}
