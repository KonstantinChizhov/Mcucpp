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

#include <stdint.h>

namespace Mcucpp
{
	template<class CharT>
	class Utf8Encoding
	{
		static const uint_fast8_t SubsequentByteMask = 0xC0;
		static const uint_fast8_t SubsequentByteValue = 0x80;

		static const uint_fast8_t SubsequentByteShift = 6;

		static const uint_fast8_t LeadingByteMask1 = 0x80;
		static const uint_fast8_t LeadingByteValue1 = 0x00;

		static const uint_fast8_t LeadingByteMask2 = 0xE0;
		static const uint_fast8_t LeadingByteValue2 = 0xC0;

		static const uint_fast8_t LeadingByteMask3 = 0xF0;
		static const uint_fast8_t LeadingByteValue3 = 0xE0;

		static const uint_fast8_t LeadingByteMask4 = 0xF8;
		static const uint_fast8_t LeadingByteValue4 = 0xF0;

	public:

		static const CharT ErrorMarkWChar = '?';
		static const char ErrorMarkChar = '?';

		template<class StrPtr>
		static CharT Decode(StrPtr &str)
		{
			uint_fast8_t c = *str++;
			uint_fast8_t charBytes = 1;
			uint_fast8_t leadingByteMask = uint_fast8_t(~LeadingByteMask1);

			if(c == 192 || c == 193 || c >= 245)
			{
				return ErrorMarkWChar;
			}

			if((c & SubsequentByteMask) == SubsequentByteValue)
			{
				return ErrorMarkWChar;
			}

			if((c & LeadingByteMask1) == LeadingByteValue1) // Leading byte 1 byte char
			{
				// do nothing
			}
			else if((c & LeadingByteMask2) == LeadingByteValue2) // Leading byte 2 byte char
			{
				charBytes = 2;
				leadingByteMask = uint_fast8_t(~LeadingByteMask2);
			} else if((c & LeadingByteMask3) == LeadingByteValue3)// Leading byte 3 byte char
			{
				charBytes = 3;
				leadingByteMask = uint_fast8_t(~LeadingByteMask3);
			}else if((c & LeadingByteMask4) == LeadingByteValue4)// Leading byte 3 byte char
			{
				charBytes = 4;
				leadingByteMask = uint_fast8_t(~LeadingByteMask4);
			}

			CharT result = c & leadingByteMask;
			for(uint_fast8_t i = 1; i < charBytes; i++)
			{
				c = *str;
				if((c & SubsequentByteMask) != SubsequentByteValue)
				{
					result = ErrorMarkWChar;
					break;
				}
				result <<= SubsequentByteShift;
				result |= c & ~SubsequentByteMask;
				++str;
			}

			return result;
		}

		template<class StrPtr>
		static void Encode(StrPtr &str, CharT c)
		{
			int_fast8_t subsequentBytes = 0;
			uint_fast8_t leadingByteMask = uint_fast8_t(~LeadingByteMask1);
			uint_fast8_t leadingByteValue = LeadingByteValue1;

			if(c <= 0x7F)
			{
				// do nothing
			}else if(c <= 0x7FF)
			{
				subsequentBytes = 1;
				leadingByteMask = uint_fast8_t(~LeadingByteMask2);
				leadingByteValue = LeadingByteValue2;
			}else if(c <= 0xFFFF)
			{
				subsequentBytes = 2;
				leadingByteMask = uint_fast8_t(~LeadingByteMask3);
				leadingByteValue = LeadingByteValue3;
			}else if(c <= 0x1FFFFF)
			{
				subsequentBytes = 3;
				leadingByteMask = uint_fast8_t(~LeadingByteMask4);
				leadingByteValue = LeadingByteValue4;
			}else
			{
				*str++ = ErrorMarkChar;
				return;
			}

			*str++ = (char(c >> (SubsequentByteShift * subsequentBytes)) & leadingByteMask) | leadingByteValue;

			for(int_fast8_t i = subsequentBytes - 1; i >= 0; --i)
			{
				*str++ = (char(c >> (SubsequentByteShift * i)) & ~SubsequentByteMask) | SubsequentByteValue;
			}
		}

		template<class StrPtr>
		static size_t StrLen(StrPtr str)
		{
			size_t result = 0;
			uint_fast8_t c;
			while((c = *str++))
			{
				uint_fast8_t charBytes;

				if(c == 192 || c == 193 || c >= 245)
					break;

				if((c & SubsequentByteMask) == SubsequentByteValue)
				{
					break;
				}

				result++;

				if((c & LeadingByteMask1) == LeadingByteValue1) // Leading byte 1 byte char
				{
					continue;
				}
				else if((c & LeadingByteMask2) == LeadingByteValue2) // Leading byte 2 byte char
				{
					charBytes = 2;
				} else if((c & LeadingByteMask3) == LeadingByteValue3)// Leading byte 3 byte char
				{
					charBytes = 3;
				}else if((c & LeadingByteMask4) == LeadingByteValue4)// Leading byte 3 byte char
				{
					charBytes = 4;
				}

				for(uint_fast8_t i = 1; i < charBytes; i++)
				{
					c = *str;
					if((c & SubsequentByteMask) != SubsequentByteValue)
					{
						return result;
					}
					++str;
				}
			}
			return result;
		}
	};
}
