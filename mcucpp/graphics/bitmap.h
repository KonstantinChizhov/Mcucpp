
#ifndef __MCUCPP_BITMAP_H__
#define __MCUCPP_BITMAP_H__

namespace Mcucpp
{
	namespace Graphics
	{
		template<class PtrT = unsigned char*>
		class BitRef
		{
		public:
			BitRef(PtrT byte, unsigned char mask)
			:_byte(byte), _mask(mask)
			{}
			operator unsigned char ()
			{
				return (*_byte & _mask) != 0;
			}
			BitRef &operator =(unsigned char value)
			{
				if(value)
					*_byte |= _mask;
				else
					*_byte &= ~_mask;
				return *this;
			}
		private:
			PtrT _byte;
			unsigned char _mask;
		};

		template<class PtrT = unsigned char*>
		class BitmapBase
		{	
		public:
			BitmapBase(PtrT data, unsigned width, unsigned height)
			:_data(data), _dim1(width), _dim2(height)
			{
				_bytesPerLine = (_dim1 + 7) >> 3;
			}
		
			BitRef<PtrT> operator()(unsigned x, unsigned y)
			{
				return BitRef<PtrT>(_data + y * _bytesPerLine + (x >> 3), 1 << (x & 7));
			}
			
			bool operator()(unsigned x, unsigned y)const
			{
				return (*(_data + y * _bytesPerLine + (x >> 3)) & (1 << (x & 7))) != 0;
			}

			unsigned Width(){return _dim1;};
			unsigned Height(){return _dim2;};

		protected:
			PtrT _data;
			unsigned _dim1, _dim2, _bytesPerLine;
		};

		typedef BitmapBase<unsigned char*> Bitmap;


		template<class PtrT = unsigned char*>
		class ColumnOrderedBitmap
		{
		public:
			ColumnOrderedBitmap(PtrT data, unsigned width, unsigned height)
			:_data(data), _dim1(width), _dim2(height)
			{
				_bytesPerLine = (_dim2 + 7) >> 3;
			}
		
			BitRef<PtrT> operator()(unsigned x, unsigned y)
			{
				return BitRef<PtrT>(_data + x * _bytesPerLine + (y >> 3), 1 << (y & 7));
			}
			
			bool operator()(unsigned x, unsigned y)const
			{
				return (*(_data + x * _bytesPerLine + (y >> 3)) & (1 << (y & 7))) != 0;
			}

		
			unsigned Width(){return _dim1;};
			unsigned Height(){return _dim2;};

		protected:
			PtrT _data;
			unsigned _dim1, _dim2, _bytesPerLine;
		};
	}
}
#endif
