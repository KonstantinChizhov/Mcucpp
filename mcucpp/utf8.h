

namespace Mcucpp
{
	template<class StrPtr, class CharT>
	class Utf8Encoding
	{
		public:
		static CharT ErrorMark = '?';
		
		CharT Read(StrPtr &str)
		{
			uint_fast8_t c = *str++;
			CharT result = 0;
			if(c & 0x80 == 0) // ASCII 1 byte
				result = c;
			else if(c & 0xE0 == 0xC0) // 2 bytes
			{
				result = c << 6;
				c = *str++;
				if(c & 0xC0 != 0x80) // bogus byte
					result = ErrorMark;
				else
					result |= c & 0xC0;
			} //else if()
			
			return result;
		}
		
	};
}
