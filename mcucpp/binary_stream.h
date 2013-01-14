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

#include <stdint.h>

namespace Mcucpp
{

	enum Endianness
	{
		BigEndian = 0,
		LittleEndian = 1,
		MixedEndian = 2
	};


	//Spi::Read<uint32_t, BigEndian>();

	template<class Source>
	class BinaryStream :public Source
	{
	public:
		using Source::Read;
		using Source::Write;

		static inline uint32_t ReadU32Be();
		static inline uint32_t ReadU32Le();
		static inline uint16_t ReadU16Be();
		static inline uint16_t ReadU16Le();

		static inline void WriteU32Be(uint32_t value);
		static inline void WriteU32Le(uint32_t value);
		static inline void WriteU16Be(uint16_t value);
		static inline void WriteU16Le(uint16_t value);

		/// Reads and discards specified number of bytes
		/// Returns last byte read
		static inline uint8_t Ignore(size_t bytes);

		/// Reads and discards specified number of bytes or until 'delim' byte is found
		/// Returns last byte read
		static inline uint8_t Ignore(size_t bytes, uint8_t delim);

		/// Reads and discards specified number of bytes while read byte is eq to 'value'
		/// Returns last byte read
		static inline uint8_t IgnoreWhile(size_t bytes, uint8_t value);

		template<class PtrType>
		static inline void Read(PtrType buffer, size_t size)
		{
			for(size_t i = 0; i < size; ++i)
			{
				*buffer = Source::Read();
				++buffer;
			}
		}

		template<class PtrType>
		static inline void Write(PtrType buffer, size_t size)
		{
			for(size_t i = 0; i < size; ++i)
			{
				Source::Write(*buffer);
				++buffer;
			}
		}
	};

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
		uint8_t value;
		for(size_t i = 0; i < bytes; ++i)
		{
			value = Source::Read();
		}
		return value;
	}

	template<class Source>
	uint8_t BinaryStream<Source>::Ignore(size_t bytes, uint8_t delim)
	{
		uint8_t value;
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
		uint8_t value;
		for(size_t i = 0; i < bytes; ++i)
		{
			value = Source::Read();
			if(value != expected)
				break;
		}
		return value;
	}
}
