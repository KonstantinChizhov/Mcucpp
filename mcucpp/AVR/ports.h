//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2010
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

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef AVR_PORTS_H
#define AVR_PORTS_H

#include <__compatibility.h>
#include <atomic.h>
#include <ioreg.h>
#include <template_utils.h>

namespace Mcucpp
{
	namespace IO
	{
		class NativePortBase
		{
			public:
				typedef uint8_t DataT;
				enum{Width=sizeof(DataT)*8};
				static const unsigned MaxBitwiseOutput = 5;
			public:
				enum Configuration
				{
					Analog = 0,
					In = 0x00,
					Out = 0x01,
					AltFunc = 0x01
				};
				
				enum PullMode
				{
					NoPullUp = 0,
					PullUp   = 1,
					PullDown = 2
				};
				
				enum DriverType
				{
					PushPull  = 0,
					OpenDrain = 0
				};
				
				enum Speed
				{
					Slow    = 0,
					Fast    = 0,
					Fastest = 0
				};
		};
		
		//Port definitions for AtTiny, AtMega families.

		template<class Out, class Dir, class In, int ID>
		class PortImplimentation :public NativePortBase
		{
			template<DataT value, DataT mask>
			static inline void SetBitWise()
			{
				if(mask == 0) return;
				if(value & mask)
					Out::Or(value & mask);
				SetBitWise<value, DataT(mask << 1)>();
			}

			template<DataT value, DataT mask>
			static inline void ClearBitWise()
			{
				if(mask == 0) return;
				if(value & mask)
					Out::And(DataT(~(value & mask)));
				ClearBitWise<value, DataT(mask << 1)>();
			}
		public:
			static void Write(DataT value)
			{
				Out::Set(value);
			}

			static void ClearAndSet(DataT clearMask, DataT value)
			{
				ATOMIC Out::AndOr(DataT(~clearMask), value);
			}

			static DataT Read()
			{
				return Out::Get();
			}

			static void Set(DataT value)
			{
				ATOMIC Out::Or(value);
			}

			static void Clear(DataT value)
			{
				ATOMIC Out::And(~value);
			}

			static void Toggle(DataT value)
			{
				ATOMIC Out::Xor(value);
			}

			static DataT PinRead()
			{
				return In::Get();
			}
			
			static void SetSpeed(DataT mask, Speed speed)
			{
				// nothing to do
			}
			
			static void SetPullUp(DataT mask, PullMode pull)
			{
				if(pull == PullUp)
				{
					ATOMIC
					{
						Dir::And(DataT(~mask));
						Out::Or(mask);
					}
				}
			}
			
			static void SetDriverType(DataT mask, DriverType driver)
			{
				// nothing to do
			}
			
			static void AltFuncNumber(DataT mask, uint8_t number)
			{
				// nothing to do
			}

			// constant interface

			template<DataT clearMask, DataT value>
			static void ClearAndSet()
			{
				const DataT bitsToSet = value & clearMask;
				const DataT bitsToClear = DataT(~value & clearMask);

				const unsigned countBitsToChange = Util::PopulatedBits<clearMask>::value;

				if(countBitsToChange <= MaxBitwiseOutput && 
					Id < 4)
				{
					SetBitWise<bitsToSet, 1>();
					ClearBitWise<bitsToClear, 1>();
				}
				else
					ATOMIC Out::AndOr(DataT(~clearMask), value);
			}

			template<DataT value>
			static void Toggle()
			{
				ATOMIC Out::Xor(value);
			}

			template<DataT value>
			static void Set()
			{
				if(Util::PopulatedBits<value>::value <= MaxBitwiseOutput && 
					Id < 4)
					SetBitWise<value, 1>();
				else
					ATOMIC Out::Or(value);
			}

			template<DataT value>
			static void Clear()
			{
				if(Util::PopulatedBits<value>::value <= MaxBitwiseOutput && 
					Id < 4)
					ClearBitWise<value, 1>();
				else
					ATOMIC Out::And(DataT(~value));
			}

			// Configuration interface

			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				STATIC_ASSERT(pin < Width);
				if(configuration)
					Dir::Or(1 << pin);
				else
					Dir::And(DataT(~(1 << pin)));
			}

			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				if(configuration)
					Dir::Or(mask);
				else
					Dir::And(DataT(~mask));
			}

			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
				if(configuration)
				{
					if(mask == (DataT)-1)
						Dir::Set(mask);
					else
						Dir::Or(mask);
				}
				else
				{
					if(mask == (DataT)-1)
						Dir::Set(DataT(~mask));
					else
						Dir::And(DataT(~mask));
				}
			}
			
			static void Enable() {}
			static void Disable() {}

			enum{Id = ID};
		};

#include "__port_def.h"

	} // IO

}// Mcucpp



#endif
