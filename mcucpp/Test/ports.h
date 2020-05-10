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

namespace Mcucpp
{
	namespace IO
	{
		class NativePortBase
		{
			public:
			enum Configuration
			{
				Analog = 0,
				In = 0x00,
				Out = 0x01,
				AltOut = 0x01,
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
				OpenDrain = 1
			};
			
			enum Speed
			{
				Slow    = 0,
				Fast    = 1,
				Fastest = 2
			};
		};


		class NullCallback
		{
		public:
			template<class PortT>
			void PortChanged()
			{

			}
		};

		namespace Test
		{
			template<class DataType, unsigned Identity, class CallbackT = NullCallback>
			class TestPort :public NativePortBase
			{
				typedef TestPort<DataType, Identity, CallbackT> Self;
				static void StoreRegs()
				{
					PrevDirReg = DirReg;
					PrevOutReg = OutReg;
				}

				static void PortCallback()
				{
					if(callback)
					callback->template PortChanged<Self>();
				}

				public:

				typedef DataType DataT;
				template<unsigned pin>
				static void SetPinConfiguration(Configuration configuration)
				{
					StoreRegs();
					STATIC_ASSERT(pin < Width);
					if(configuration)
						DirReg |= 1 << pin;
					else
						DirReg &= ~(1 << pin);
					PortCallback();
				}

				static void SetConfiguration(DataT mask, Configuration configuration)
				{
					StoreRegs();
					if(configuration)
						DirReg |= mask;
					else
						DirReg &= ~mask;
					PortCallback();
				}

				template<DataT mask, Configuration configuration>
				static void SetConfiguration()
				{
					StoreRegs();
					if(configuration)
						DirReg |= mask;
					else
						DirReg &= ~mask;
					PortCallback();
				}
				
				static void SetSpeed(DataT mask, Speed speed)
				{
					// TODO
				}
				
				static void SetPullUp(DataT mask, PullMode pull)
				{
					// TODO
				}
				
				static void SetDriverType(DataT mask, DriverType driver)
				{
					// TODO
				}
				
				static void AltFuncNumber(DataT mask, uint8_t number)
				{
					// TODO
				}

				static void Write(DataT value)
				{
					StoreRegs();
					OutReg = value;
					PortCallback();
				}
				static void ClearAndSet(DataT clearMask, DataT value)
				{
					StoreRegs();
					OutReg &= ~clearMask;
					OutReg |= value;
					PortCallback();
				}
				static DataT Read()
				{
					return OutReg;
				}
				static void Set(DataT value)
				{
					StoreRegs();
					OutReg |= value;
					PortCallback();
				}
				static void Clear(DataT value)
				{
					StoreRegs();
					OutReg &= ~value;
					PortCallback();
				}
				static void Toggle(DataT value)
				{
					StoreRegs();
					OutReg ^= value;
					PortCallback();
				}
				static DataT PinRead()
				{
					return InReg;
				}

				template<DataT value>
				static void Write()
				{
					StoreRegs();
					OutReg = value;
					PortCallback();
				}

				template<DataT clearMask, DataT value>
				static void ClearAndSet()
				{
					StoreRegs();
					OutReg &= ~clearMask;
					OutReg |= value;
					PortCallback();
				}

				template<DataT value>
				static void Set()
				{
					StoreRegs();
					OutReg |= value;
					PortCallback();
				}

				template<DataT value>
				static void Clear()
				{
					StoreRegs();
					OutReg &= ~value;
					PortCallback();
				}

				template<DataT value>
				static void Toggle()
				{
					StoreRegs();
					OutReg ^= value;
					PortCallback();
				}
				
				static void Enable()
				{
					enabled = true;
				}
				
				static void Disable()
				{
					enabled = false;
				}

				enum{Id = Identity};
				enum{Width=sizeof(DataT)*8};
				enum{Length=Width};

				volatile static DataType OutReg;
				volatile static DataType DirReg;
				volatile static DataType InReg;
				volatile static DataType PrevOutReg;
				volatile static DataType PrevDirReg;

				volatile static bool enabled;

				static CallbackT *callback;
			};

			template<class DataType, unsigned Identity, class CallbackT>
			volatile DataType TestPort<DataType, Identity, CallbackT>::OutReg;

			template<class DataType, unsigned Identity, class CallbackT>
			volatile DataType TestPort<DataType, Identity, CallbackT>::DirReg;

			template<class DataType, unsigned Identity, class CallbackT>
			volatile DataType TestPort<DataType, Identity, CallbackT>::InReg;

			template<class DataType, unsigned Identity, class CallbackT>
			volatile DataType TestPort<DataType, Identity, CallbackT>::PrevOutReg;

			template<class DataType, unsigned Identity, class CallbackT>
			volatile DataType TestPort<DataType, Identity, CallbackT>::PrevDirReg;

			template<class DataType, unsigned Identity, class CallbackT>
			CallbackT *TestPort<DataType, Identity, CallbackT>::callback = 0;

			template<class DataType, unsigned Identity, class CallbackT>
			volatile bool TestPort<DataType, Identity, CallbackT>::enabled = false;
		}
		
		typedef Test::TestPort<uint32_t, 'A'> Porta;
		typedef Test::TestPort<uint32_t, 'B'> Portb;
		typedef Test::TestPort<uint32_t, 'C'> Portc;
		typedef Test::TestPort<uint32_t, 'D'> Portd;
		typedef Test::TestPort<uint32_t, 'E'> Porte;
		typedef Test::TestPort<uint32_t, 'F'> Portf;
		
		#define MCUCPP_HAS_PORTA
		#define MCUCPP_HAS_PORTB
		#define MCUCPP_HAS_PORTC
		#define MCUCPP_HAS_PORTD
		#define MCUCPP_HAS_PORTE
		#define MCUCPP_HAS_PORTF
	}
}
