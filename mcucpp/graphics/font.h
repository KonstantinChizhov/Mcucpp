#ifndef __MCUCPP_FONT_H__
#define __MCUCPP_FONT_H__

#include <graphics/bitmap.h>
#include <stddef.h>

namespace Mcucpp
{
	namespace Graphics
	{
		struct CharInfo
		{
			uint8_t width; 
			size_t offset;
		};

		struct FontInfo
		{
			wchar_t	startChar,
					endChar;
			const uint8_t *	rawDataPtr;
			const CharInfo* charInfos;
		};

		template<class BitmapBytePtrT=uint8_t*, class CharInfoPtrT=const CharInfo*>
		class RastrFont
		{
		public:
			typedef ColumnOrderedBitmap<BitmapBytePtrT> CharType;
			RastrFont(const FontInfo *charSets, 
						uint_fast8_t charSetsCount,
						DimensionType charWidth, 
						DimensionType charHeight, 
						wchar_t unknownChar = '?');

			const ColumnOrderedBitmap<BitmapBytePtrT> GetChar(char c)const
				{return GetChar(static_cast<wchar_t>((uint8_t)c));}
			const ColumnOrderedBitmap<BitmapBytePtrT> GetChar(wchar_t c)const;
		protected:
			const FontInfo *_charSets;
			uint_fast8_t _charSetsCount;
			wchar_t _unknownChar;
			DimensionType _width, _height;
			wchar_t _charSizeBytes;
		};


		template<class BitmapBytePtrT, class CharInfoPtrT>
		RastrFont<BitmapBytePtrT, CharInfoPtrT>::RastrFont(const FontInfo *charSets, 
						uint_fast8_t charSetsCount,
						DimensionType charWidth, 
						DimensionType charHeight, 
						wchar_t unknownChar)
		:_charSets(charSets),
		_charSetsCount(charSetsCount),
		_unknownChar(unknownChar),
		_width(charWidth), 
		_height(charHeight),
		_charSizeBytes(((charHeight + 7) >> 3) * charWidth)
		{}
		
		template<class BitmapBytePtrT, class CharInfoPtrT>
		const ColumnOrderedBitmap<BitmapBytePtrT> RastrFont<BitmapBytePtrT, CharInfoPtrT>::GetChar(wchar_t c)const
		{
			uint_fast8_t i = 0;
			for(; i < _charSetsCount &&
				((unsigned)_charSets[i].startChar < (unsigned)c && 
				(unsigned)_charSets[i].endChar < (unsigned)c);
				i++);
			if(i == _charSetsCount)
			{
				i = 0;
				c = _unknownChar;
			}

			size_t offset, width;
			c -= _charSets[i].startChar;
			
			if(_charSets[i].charInfos)
			{
				CharInfo ci = *CharInfoPtrT(&_charSets[i].charInfos[(unsigned)c]);
				width = ci.width;
				offset = ci.offset;
			}
			else 
			{
				width = _width;
				offset = _charSizeBytes * (unsigned)c;
			}

			return CharType(_charSets[i].rawDataPtr + offset, width, _height);
		}
	}
}
#endif
