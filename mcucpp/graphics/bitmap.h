
#ifndef __MCUCPP_BITMAP_H__
#define __MCUCPP_BITMAP_H__

#include <stdint.h>

namespace Mcucpp
{
	namespace Graphics
	{
		typedef uint_fast16_t DimensionType;

		template<class PtrT = uint8_t*>
		class BitRef
		{
		public:
			BitRef(PtrT byte, uint8_t mask)
			:_byte(byte), _mask(mask)
			{}
			operator uint_fast8_t ()
			{
				return (*_byte & _mask) != 0;
			}
			BitRef &operator =(uint_fast8_t value)
			{
				if(value)
					*_byte |= _mask;
				else
					*_byte &= ~_mask;
				return *this;
			}
		private:
			PtrT _byte;
			uint8_t _mask;
		};

		template<class PtrT = uint8_t*>
		class RowOrderedBitmap
		{
		public:
			RowOrderedBitmap(PtrT data, DimensionType width, DimensionType height)
			:_data(data), _width(width), _height(height)
			{
				_bytesPerLine = (_width + 7) >> 3;
			}
		
			BitRef<PtrT> operator()(DimensionType x, DimensionType y)
			{
				return BitRef<PtrT>(_data + y * _bytesPerLine + (x >> 3), 1 << (x & 7));
			}
			
			uint8_t operator()(DimensionType x, DimensionType y)const
			{
				return (*(_data + y * _bytesPerLine + (x >> 3)) & (1 << (x & 7))) != 0;
			}

			DimensionType Width()const{return _width;}
			DimensionType Height()const{return _height;}

		protected:
			PtrT _data;
			DimensionType _width, _height, _bytesPerLine;
		};


		template<class PtrT = unsigned char*>
		class ColumnOrderedBitmap
		{
		public:
			ColumnOrderedBitmap(PtrT data, DimensionType width, DimensionType height)
			:_data(data), _width(width), _height(height)
			{
				_bytesPerLine = (_height + 7) >> 3;
			}
		
			BitRef<PtrT> operator()(DimensionType x, DimensionType y)
			{
				return BitRef<PtrT>(_data + x * _bytesPerLine + (y >> 3), 1 << (y & 7));
			}
			
			uint8_t operator()(DimensionType x, DimensionType y)const
			{
				return (*(_data + x * _bytesPerLine + (y >> 3)) & (1 << (y & 7))) != 0;
			}

			DimensionType Width()const{return _width;}
			DimensionType Height()const{return _height;}

		protected:
			PtrT _data;
			DimensionType _width, _height, _bytesPerLine;
		};
	}
}
#endif
