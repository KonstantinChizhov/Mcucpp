

namespace Mcucpp
{
	template<class StrPtr, class CharT>
	class Utf8Encoding
	{
		static const CharT SubsequentByteMask = 0xC0;
		static const CharT SubsequentByteValue = 0x80;

		static const uint_fast8_t SubsequentByteShift = 6;

		static const CharT LeadingByteMask1 = 0x80;
		static const CharT LeadingByteValue1 = 0x00;

		static const CharT LeadingByteMask2 = 0xE0;
		static const CharT LeadingByteValue2 = 0xC0;

		static const CharT LeadingByteMask3 = 0xF0;
		static const CharT LeadingByteValue3 = 0xE0;

		static const CharT LeadingByteMask4 = 0xF8;
		static const CharT LeadingByteValue4 = 0xF0;

	public:
		static const CharT ErrorMarkChar = '?';

		static CharT Read(StrPtr &str)
		{
			uint_fast8_t c = *str++;
			uint_fast8_t charBytes = 1;
			uint_fast8_t leadingByteMask = LeadingByteValue1;

			if(c == 192 || c == 193 || c >= 245)
			{
				return ErrorMarkChar;
			}

			if((c & SubsequentByteMask) == SubsequentByteValue)
			{
				return ErrorMarkChar;
			}

			if((c & LeadingByteMask1) == LeadingByteValue1) // Leading byte 1 byte char
			{
				// do nothing
			}
			else if((c & LeadingByteMask2) == LeadingByteValue2) // Leading byte 2 byte char
			{
				charBytes = 2;
				leadingByteMask = LeadingByteMask2;
			} else if((c & LeadingByteMask3) == LeadingByteValue3)// Leading byte 3 byte char
			{
				charBytes = 3;
				leadingByteMask = LeadingByteMask3;
			}else if((c & LeadingByteMask4) == LeadingByteValue4)// Leading byte 3 byte char
			{
				charBytes = 4;
				leadingByteMask = LeadingByteMask4;
			}

			CharT result = c & ~leadingByteMask;
			for(uint_fast8_t i = 1; i < charBytes; i++)
			{
				c = *str;
				if((c & SubsequentByteMask) != SubsequentByteValue)
				{
					result = ErrorMarkChar;
					break;
				}
				result <<= SubsequentByteShift;
				result |= c & SubsequentByteMask;
				str++;
			}

			return result;
		}

	};
}
