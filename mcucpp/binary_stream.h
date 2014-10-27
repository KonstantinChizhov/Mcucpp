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
#include <stdint.h>

namespace Mcucpp
{

	enum Endianness
	{
		BigEndian = 0,
		LittleEndian = 1,
		MixedEndian = 2
	};

	template<class Source>
	class BinaryStream :public Source
	{
	public:
		BinaryStream()
		{}

		template<class T1>
		BinaryStream(T1 arg1)
			:Source(arg1)
		{
		}

		template<class T1, class T2>
		BinaryStream(T1 arg1, T2 arg2)
			:Source(arg1, arg2)
		{
		}

		template<class T1, class T2, class T3>
		BinaryStream(T1 arg1, T2 arg2, T3 arg3)
			:Source(arg1, arg2, arg3)
		{
		}

		inline uint32_t ReadU32Be();
		inline uint32_t ReadU32Le();
		inline uint16_t ReadU16Be();
		inline uint16_t ReadU16Le();
		inline uint8_t ReadU8(){ return Source::Read(); }
		inline uint8_t Read(){ return Source::Read(); }

		inline void WriteU32Be(uint32_t value);
		inline void WriteU32Le(uint32_t value);
		inline void WriteU16Be(uint16_t value);
		inline void WriteU16Le(uint16_t value);
		inline void WriteU8(uint8_t value){ Source::Write(value); }
		inline void Write(uint8_t value){ Source::Write(value); }

		/// Reads and discards specified number of bytes
		/// Returns last byte read
		inline uint8_t Ignore(size_t bytes);

		/// Reads and discards specified number of bytes or until 'delim' byte is found
		/// Returns last byte read
		inline uint8_t Ignore(size_t bytes, uint8_t delim);

		/// Reads and discards specified number of bytes while read byte is eq to 'value'
		/// Returns last byte read
		inline uint8_t IgnoreWhile(size_t bytes, uint8_t value);

		template<class PtrType>
		inline void Read(PtrType buffer, size_t size)
		{
			for(size_t i = 0; i < size; ++i)
			{
				*buffer = Source::Read();
				++buffer;
			}
		}

		template<class PtrType>
		inline void Write(PtrType buffer, size_t size)
		{
			for(size_t i = 0; i < size; ++i)
			{
				Source::Write(*buffer);
				++buffer;
			}
		}
	};

	
	template<class Source>
	class BinaryStreamAdapter
	{
		Source &_source;
	public:
		BinaryStreamAdapter(Source &source)
			:_source(source)
		{}
		
		inline uint8_t Read(){ return _source.Read(); }
		inline void Write(uint8_t value){ _source.Write(value); }
		
		inline uint32_t ReadU32Be();
		inline uint32_t ReadU32Le();
		inline uint16_t ReadU16Be();
		inline uint16_t ReadU16Le();
		inline uint8_t ReadU8(){ return _source.Read(); }

		inline void WriteU32Be(uint32_t value);
		inline void WriteU32Le(uint32_t value);
		inline void WriteU16Be(uint16_t value);
		inline void WriteU16Le(uint16_t value);
		inline void WriteU8(uint8_t value){ _source.Write(value); }

		/// Reads and discards specified number of bytes
		/// Returns last byte read
		inline uint8_t Ignore(size_t bytes);

		/// Reads and discards specified number of bytes or until 'delim' byte is found
		/// Returns last byte read
		inline uint8_t Ignore(size_t bytes, uint8_t delim);

		/// Reads and discards specified number of bytes while read byte is eq to 'value'
		/// Returns last byte read
		inline uint8_t IgnoreWhile(size_t bytes, uint8_t value);

		template<class PtrType>
		inline void Read(PtrType buffer, size_t size)
		{
			for(size_t i = 0; i < size; ++i)
			{
				*buffer = _source.Read();
				++buffer;
			}
		}

		template<class PtrType>
		inline void Write(PtrType buffer, size_t size)
		{
			for(size_t i = 0; i < size; ++i)
			{
				_source.Write(*buffer);
				++buffer;
			}
		}
		
		Source *operator->(){return &_source;}
	};
	
	//==========================================================================================
	// BinaryStream members
	//==========================================================================================
	template<class Source>
	uint32_t BinaryStream<Source>::ReadU32Be()
	{
		uint32_t result = uint32_t(Source::Read()) << 24;
		result |= uint32_t(Source::Read()) << 16;
		result |= uint32_t(Source::Read()) << 8;
		result |= uint32_t(Source::Read()) ;
		return result;
	}

	template<class Source>
	uint32_t BinaryStream<Source>::ReadU32Le()
	{
		uint32_t result = uint32_t(Source::Read());
		result |= uint32_t(Source::Read()) << 8;
		result |= uint32_t(Source::Read()) << 16;
		result |= uint32_t(Source::Read()) << 24;
		return result;
	}

	template<class Source>
	uint16_t BinaryStream<Source>::ReadU16Be()
	{
		uint16_t result = uint16_t(Source::Read()) << 8;
		result |= uint16_t(Source::Read());
		return result;
	}

	template<class Source>
	uint16_t BinaryStream<Source>::ReadU16Le()
	{
		uint16_t result = uint16_t(Source::Read());
		result |= uint16_t(Source::Read()) << 8;
		return result;
	}

	template<class Source>
	void BinaryStream<Source>::WriteU32Be(uint32_t value)
	{
		Source::Write(uint8_t(value >> 24));
		Source::Write(uint8_t(value >> 16));
		Source::Write(uint8_t(value >> 8));
		Source::Write(uint8_t(value));
	}

	template<class Source>
	void BinaryStream<Source>::WriteU32Le(uint32_t value)
	{
		Source::Write(uint8_t(value));
		Source::Write(uint8_t(value >> 8));
		Source::Write(uint8_t(value >> 16));
		Source::Write(uint8_t(value >> 24));
	}

	template<class Source>
	void BinaryStream<Source>::WriteU16Be(uint16_t value)
	{
		Source::Write(uint8_t(value >> 8));
		Source::Write(uint8_t(value));
	}

	template<class Source>
	void BinaryStream<Source>::WriteU16Le(uint16_t value)
	{
		Source::Write(uint8_t(value));
		Source::Write(uint8_t(value >> 8));
	}

	template<class Source>
	uint8_t BinaryStream<Source>::Ignore(size_t bytes)
	{
		uint8_t value = 0;
		for(size_t i = 0; i < bytes; ++i)
		{
			value = Source::Read();
		}
		return value;
	}

	template<class Source>
	uint8_t BinaryStream<Source>::Ignore(size_t bytes, uint8_t delim)
	{
		uint8_t value = 0;
		for(size_t i = 0; i < bytes; ++i)
		{
			value = Source::Read();
			if(value == delim)
				break;
		}
		return value;
	}

	template<class Source>
	uint8_t BinaryStream<Source>::IgnoreWhile(size_t bytes, uint8_t expected)
	{
		uint8_t value = expected;
		for(size_t i = 0; i < bytes; ++i)
		{
			value = Source::Read();
			if(value != expected)
				break;
		}
		return value;
	}
	
	//==========================================================================================
	// BinaryStreamAdapter members
	//==========================================================================================
	
	template<class Source>
	uint32_t BinaryStreamAdapter<Source>::ReadU32Be()
	{
		uint32_t result = uint32_t(_source.Read()) << 24;
		result |= uint32_t(_source.Read()) << 16;
		result |= uint32_t(_source.Read()) << 8;
		result |= uint32_t(_source.Read()) ;
		return result;
	}

	template<class Source>
	uint32_t BinaryStreamAdapter<Source>::ReadU32Le()
	{
		uint32_t result = uint32_t(_source.Read());
		result |= uint32_t(_source.Read()) << 8;
		result |= uint32_t(_source.Read()) << 16;
		result |= uint32_t(_source.Read()) << 24;
		return result;
	}

	template<class Source>
	uint16_t BinaryStreamAdapter<Source>::ReadU16Be()
	{
		uint16_t result = uint16_t(_source.Read()) << 8;
		result |= uint16_t(_source.Read());
		return result;
	}

	template<class Source>
	uint16_t BinaryStreamAdapter<Source>::ReadU16Le()
	{
		uint16_t result = uint16_t(_source.Read());
		result |= uint16_t(_source.Read()) << 8;
		return result;
	}

	template<class Source>
	void BinaryStreamAdapter<Source>::WriteU32Be(uint32_t value)
	{
		_source.Write(uint8_t(value >> 24));
		_source.Write(uint8_t(value >> 16));
		_source.Write(uint8_t(value >> 8));
		_source.Write(uint8_t(value));
	}

	template<class Source>
	void BinaryStreamAdapter<Source>::WriteU32Le(uint32_t value)
	{
		_source.Write(uint8_t(value));
		_source.Write(uint8_t(value >> 8));
		_source.Write(uint8_t(value >> 16));
		_source.Write(uint8_t(value >> 24));
	}

	template<class Source>
	void BinaryStreamAdapter<Source>::WriteU16Be(uint16_t value)
	{
		_source.Write(uint8_t(value >> 8));
		_source.Write(uint8_t(value));
	}

	template<class Source>
	void BinaryStreamAdapter<Source>::WriteU16Le(uint16_t value)
	{
		_source.Write(uint8_t(value));
		_source.Write(uint8_t(value >> 8));
	}

	template<class Source>
	uint8_t BinaryStreamAdapter<Source>::Ignore(size_t bytes)
	{
		uint8_t value = 0;
		for(size_t i = 0; i < bytes; ++i)
		{
			value = _source.Read();
		}
		return value;
	}

	template<class Source>
	uint8_t BinaryStreamAdapter<Source>::Ignore(size_t bytes, uint8_t delim)
	{
		uint8_t value = 0;
		for(size_t i = 0; i < bytes; ++i)
		{
			value = _source.Read();
			if(value == delim)
				break;
		}
		return value;
	}

	template<class Source>
	uint8_t BinaryStreamAdapter<Source>::IgnoreWhile(size_t bytes, uint8_t expected)
	{
		uint8_t value = expected;
		for(size_t i = 0; i < bytes; ++i)
		{
			value = _source.Read();
			if(value != expected)
				break;
		}
		return value;
	}
}
