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

	namespace Test
	{
		template<class DataType, unsigned Identity>
		class TestPort :public TestPortBase
		{
			public:

            typedef DataType DataT;
            typedef TestPortBase Base;
			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if(configuration)
					DirReg |= 1 << pin;
				else
					DirReg &= ~(1 << pin);
			}

			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				if(configuration)
					DirReg |= mask;
				else
					DirReg &= ~mask;
			}

			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
				if(configuration)
					DirReg |= mask;
				else
					DirReg &= ~mask;
			}

			static void Write(DataT value)
			{
				OutReg = value;
			}
			static void ClearAndSet(DataT clearMask, DataT value)
			{
				OutReg &= ~clearMask;
				OutReg |= value;
			}
			static DataT Read()
			{
				return OutReg;
			}
			static void Set(DataT value)
			{
				OutReg |= value;
			}
			static void Clear(DataT value)
			{
				OutReg &= ~value;
			}
			static void Toggle(DataT value)
			{
				OutReg ^= value;
			}
			static DataT PinRead()
			{
				return InReg;
			}

			template<DataT value>
			static void Write()
			{
				OutReg = value;
			}

			template<DataT clearMask, DataT value>
			static void ClearAndSet()
			{
				OutReg &= ~clearMask;
				OutReg |= value;
			}


			template<DataT value>
			static void Set()
			{
				OutReg |= value;
			}

			template<DataT value>
			static void Clear()
			{
				OutReg &= ~value;
			}

			template<DataT value>
			static void Toggle()
			{
				OutReg ^= value;
			}

			enum{Id = Identity};
			enum{Width=sizeof(DataT)*8};

			volatile static DataType OutReg;
			volatile static DataType DirReg;
			volatile static DataType InReg;
		};

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::OutReg;

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::DirReg;

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::InReg;
	}
}
