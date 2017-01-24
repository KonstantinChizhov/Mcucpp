//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
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
#include <string.h>
#include <static_assert.h>
#include <template_utils.h>

namespace Mcucpp
{
namespace Net
{
	template<unsigned BytesCount>
	class NetAddress
	{
		uint8_t _value[BytesCount];
		
		explicit NetAddress(bool broadcast)
		{
			if(broadcast)
			{
				Util::fill_n(_value, sizeof(_value), 0xff);
			}
		}
	public:
		unsigned Length() const { return BytesCount; }
		
		NetAddress()
		{
			Util::fill_n(_value, sizeof(_value), 0);
		}
		
		
		explicit NetAddress(const uint8_t *value)
		{
			Util::copy(value, _value, sizeof(_value));
		}
		
		explicit NetAddress(uint8_t a)
		{
			STATIC_ASSERT(BytesCount == 1);
			_value[0] = a;
		}
		
		NetAddress(uint8_t a, uint8_t b)
		{
			STATIC_ASSERT(BytesCount == 2);
			_value[0] = a;
			_value[1] = b;
		}
		
		NetAddress(uint8_t a, uint8_t b, uint8_t c)
		{
			STATIC_ASSERT(BytesCount == 3);
			_value[0] = a;
			_value[1] = b;
			_value[2] = c;
		}
		
		NetAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
		{
			STATIC_ASSERT(BytesCount == 4);
			_value[0] = a;
			_value[1] = b;
			_value[2] = c;
			_value[3] = d;
		}
		
		NetAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e)
		{
			STATIC_ASSERT(BytesCount == 5);
			_value[0] = a;
			_value[1] = b;
			_value[2] = c;
			_value[3] = d;
			_value[4] = e;
		}
		
		NetAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f)
		{
			STATIC_ASSERT(BytesCount == 6);
			_value[0] = a;
			_value[1] = b;
			_value[2] = c;
			_value[3] = d;
			_value[4] = e;
			_value[5] = f;
		}
		
		NetAddress(const NetAddress & rhs)
		{
			Util::copy(rhs._value, _value, sizeof(_value));
		}
		
		NetAddress & operator=(const NetAddress & rhs)
		{
			Util::copy(rhs._value, _value, sizeof(_value));
			return *this;
		}
		operator const uint8_t*()const
		{
			return &_value[0];
		}
		bool operator==(const NetAddress & rhs)const
		{
			return Util::equal(_value, _value + sizeof(_value), rhs._value);
		}
		bool operator!=(const NetAddress & rhs)const
		{
			return !Util::equal(_value, _value + sizeof(_value), rhs._value);
		}
		uint8_t& operator[](int index)
		{
			return _value[index];
		}
		uint8_t operator[](int index)const
		{
			return _value[index];
		}
		
		NetAddress operator & (const NetAddress & rhs)const
		{
			NetAddress result(false);
			for(unsigned i = 0; i < sizeof(_value); i++)
				result._value[i] = _value[i] & rhs._value[i];
			return result;
		}
		
		NetAddress operator | (const NetAddress & rhs)const
		{
			NetAddress result(false);
			for(unsigned i = 0; i < sizeof(_value); i++)
				result._value[i] = _value[i] | rhs._value[i];
			return result;
		}
		
		NetAddress operator ^ (const NetAddress & rhs)const
		{
			NetAddress result(false);
			for(unsigned i = 0; i < sizeof(_value); i++)
				result._value[i] = _value[i] ^ rhs._value[i];
			return result;
		}
		
		bool IsBroadcast() const
		{
			for(unsigned i = 0; i < sizeof(_value); i++)
			{
				if(_value[i] != 0xff)
					return true;
			}
			return false;
		}
		
		bool IsEmpty() const
		{
			for(unsigned i = 0; i < sizeof(_value); i++)
			{
				if(_value[i] != 0)
					return true;
			}
			return false;
		}
		
		static const NetAddress& Broadcast()
		{
			static NetAddress broadcast(true);
			return broadcast;
		}
	};
	
	template<>
	class NetAddress<4>
	{
		uint32_t _value;
		
		explicit NetAddress(bool broadcast)
		{
			if(broadcast)
			{
				_value = 0xffffffff;
			}
		}
	public:
		unsigned Length() const { return 4; }
		
		NetAddress()
			:_value(0)
		{
			
		}
		
		explicit NetAddress(uint32_t v)
		{
			_value = v;
		}
		
		explicit NetAddress(const uint8_t *value)
		{
			_value = ((uint32_t)value[0] << 0) | ((uint32_t)value[1] << 8) |
					((uint32_t)value[2] << 16) | ((uint32_t)value[3] << 24);
		}
		
		NetAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
		{
			_value = ((uint32_t)a << 0) | ((uint32_t)b << 8) |
					((uint32_t)c << 16) | ((uint32_t)d << 24);
		}
		
		NetAddress(const NetAddress<4> & rhs)
		{
			_value = rhs._value;
		}
		
		NetAddress & operator=(const NetAddress<4> & rhs)
		{
			_value = rhs._value;
			return *this;
		}
		
		bool operator==(const NetAddress<4> & rhs)const
		{
			return _value == rhs._value;
		}
		
		bool operator!=(const NetAddress<4> & rhs)const
		{
			return _value != rhs._value;
		}
		
		// uint8_t& operator[](int index)
		// {
			// return (uint8_t)(_value >> index*8);
		// }
		
		uint32_t ToInt32() const
		{
			return _value;
		}
		
		uint8_t operator[](int index)const
		{
			return (uint8_t)(_value >> index*8);
		}
		
		NetAddress operator & (const NetAddress & rhs)const
		{
			NetAddress result(false);
			result._value = _value & rhs._value;
			return result;
		}
		
		NetAddress operator | (const NetAddress & rhs)const
		{
			NetAddress result(false);
			result._value = _value | rhs._value;
			return result;
		}
		
		NetAddress operator ^ (const NetAddress & rhs)const
		{
			NetAddress result(false);
			result._value = _value ^ rhs._value;
			return result;
		}
		
		bool IsBroadcast() const
		{
			return _value == 0xffffffff;
		}
		
		bool IsEmpty() const
		{
			return _value == 0;
		}
		
		static const NetAddress& Broadcast()
		{
			static NetAddress broadcast(true);
			return broadcast;
		}
	};
	
	
	typedef NetAddress<6> MacAddr;
	typedef NetAddress<4> IpAddr;
}
}