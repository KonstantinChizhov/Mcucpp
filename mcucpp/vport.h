//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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
#include <ports.h>

namespace Mcucpp
{
namespace IO
{

	class VPortBase :public NativePortBase
	{
		VPortBase(VPortBase &);
		VPortBase &operator=(VPortBase &);
	public:
		VPortBase()
		{

		}
		virtual void Write(DataT value)const=0;
		virtual DataT Read()const=0;
		virtual void Set(DataT value)const=0;
		virtual void Clear(DataT value)const=0;
		virtual DataT PinRead()const=0;
		virtual void SetConfiguration(DataT mask, Configuration configuration)const=0;
	};
	
	template<class Port>
	class VPort: public VPortBase
	{
	public:
		virtual void Write(DataT value)const
		{
			Port::Write(value);
		}
		
		virtual DataT Read()const
		{
			return Port::Read();
		}
		
		virtual void Set(DataT value)const
		{
			Port::Set(value);
		}
		
		virtual void Clear(DataT value)const
		{
			Port::Clear(value);
		}

		virtual DataT PinRead()const
		{
			return Port::PinRead();
		}
		
		virtual void SetConfiguration(DataT mask, VPortBase::Configuration configuration)const
		{
			Port::SetConfiguration(mask, configuration);
		}
	};
	
	class VPin 
	{
	public:
		//const uint8_t Number;
		const uint8_t Mask;
		const VPortBase &Port;
		typedef NativePortBase ConfigPort;
		typedef NativePortBase::Configuration Configuration;
		
		VPin(VPortBase &port, uint8_t number)
		:	//Number(number), 
			Mask(1 << number), 
			Port(port)
		{	}
		
		void Set()
		{
			Port.Set(Mask);
		}
		
		void Set(bool val)
		{
			if(val)
				Port.Set(Mask);
			else
				Port.Clear(Mask);
		}
		
		void SetDir(bool val)
		{
			if(val)
				Port.SetConfiguration(Mask, ConfigPort::Out);
			else
				Port.SetConfiguration(Mask, ConfigPort::In);
		}

		void Clear()
		{
			Port.Clear(Mask);
		}
		
		void SetDirRead()
		{
			Port.SetConfiguration(Mask, ConfigPort::In);
		}
		
		void SetDirWrite()
		{
			Port.SetConfiguration(Mask, ConfigPort::Out);
		}
		
		void SetConfiguration(Configuration configuration)
		{
			Port.SetConfiguration(Mask, configuration);
		}

		bool IsSet()
		{
			return Port.PinRead() & Mask;
		}
	};
}
}

