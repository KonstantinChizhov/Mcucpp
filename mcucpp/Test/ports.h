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

namespace Mcucpp
{
	namespace IO
	{
		class TestPortBase :public GpioBase
		{
			public:
			enum Configuration
			{
				AnalogIn = 0,
				In = 0x00,
				PullUpOrDownIn = 0x00,
				Out = 0x01,
				AltOut = 0x01,
			};

			static Configuration MapConfiguration(GenericConfiguration config)
			{
				if(config & GpioBase::Out)
					return Out;
				return In;
			}

			template<GenericConfiguration config>
			struct MapConfigurationConst
			{
				static const Configuration value = In;
			};
		};

		template<> struct TestPortBase::MapConfigurationConst<GpioBase::Out>{static const Configuration value = Out;};
		template<> struct TestPortBase::MapConfigurationConst<GpioBase::OpenDrainOut>{static const Configuration value = Out;};
		template<> struct TestPortBase::MapConfigurationConst<GpioBase::AltOut>{static const Configuration value = Out;};
		template<> struct TestPortBase::MapConfigurationConst<GpioBase::AltOpenDrain>{static const Configuration value = Out;};

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
			class TestPort :public TestPortBase
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
				typedef TestPortBase Base;
				template<unsigned pin>
				static void SetPinConfiguration(Configuration configuration)
				{
					StoreRegs();
					BOOST_STATIC_ASSERT(pin < Width);
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

				enum{Id = Identity};
				enum{Width=sizeof(DataT)*8};
				enum{Length=Width};

				volatile static DataType OutReg;
				volatile static DataType DirReg;
				volatile static DataType InReg;
				volatile static DataType PrevOutReg;
				volatile static DataType PrevDirReg;

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
		}
	}
}
