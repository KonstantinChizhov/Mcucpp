#ifndef __MCUCPP_FONT_H__
#define __MCUCPP_FONT_H__

#include <graphics/bitmap.h>

namespace Mcucpp
{
	namespace Graphics
	{
		template<class BytePtrT>
		class RastrFont
		{
		public:
			typedef ColumnOrderedBitmap<BytePtrT> CharType;
			RastrFont(BytePtrT fontData, unsigned charWidth, unsigned charHeight);
			ColumnOrderedBitmap<BytePtrT> GetChar(char c);
		protected:
			BytePtrT _data;
			unsigned _width, _height;
		};

		template<class BytePtrT>
		RastrFont<BytePtrT>::RastrFont(BytePtrT fontData, unsigned charWidth, unsigned charHeight)
		:_data(fontData), _width(charWidth), _height(charHeight)
		{
			
		}
		
		template<class BytePtrT>
		ColumnOrderedBitmap<BytePtrT> RastrFont<BytePtrT>::GetChar(char c)
		{
			unsigned offset = ((_height + 7) >> 3) * _width * (c - 32);
			return ColumnOrderedBitmap<BytePtrT>(_data + offset, _width, _height);
		}
	}
}
#endif
