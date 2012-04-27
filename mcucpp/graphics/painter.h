
#ifndef __MCUCPP_PAINTER_H__
#define __MCUCPP_PAINTER_H__

#include <template_utils.h>
#include <static_assert.h>

namespace Mcucpp
{
	namespace Graphics
	{
		using namespace Util;

		class Checker
		{
		public:
			Checker(unsigned width, unsigned height)
					:_width(width), _height(height)
			{}
			uint8_t operator()(unsigned x, unsigned y)
			{
				return x & 1 ^ y & 1;
			}
			unsigned Width(){return _width;}
			unsigned Height(){return _height;}
		protected:
			unsigned _width, _height;
		};

		class Fill
		{
		public:
			Fill(unsigned width, unsigned height)
					:_width(width), _height(height)
			{}
			uint8_t operator()(unsigned x, unsigned y)
			{
				return 1;
			}
			unsigned Width(){return _width;}
			unsigned Height(){return _height;}
		protected:
			unsigned _width, _height;
		};
	
		template<class Display>
		class Painter
		{
			BOOST_STATIC_ASSERT(IsSigned<typename Display::Coord>::value);
		public:
			typedef typename Display::Coord Coord;
			typedef typename Display::Color Color;

			Painter()
				:_color(Display::DefaultColor), _penWidth(1)
			{
				
			}
			void SetPenWidth(Coord w);
			void DrawLine(Coord x1, Coord y1, Coord x2, Coord y2);
			void DrawCircle(Coord x, Coord y, Coord r);
			void DrawRect(Coord left, Coord top, Coord width, Coord height);
			void FillRect(Coord left, Coord top, Coord width, Coord height);
			template<class Bitmap>
			void DrawBitmap(Bitmap &pic, Coord x, Coord y);
			template<class Font>
			void DrawText(const char *str, Coord x, Coord y, Font &font);
		protected:
			void CircleHelper(Coord x, Coord y, Coord dx, Coord dy);
			Color _color;
			Coord _penWidth;
			static const Coord MaxPenWidth = 10;
		};

		template<class Display>
		void Painter<Display>::SetPenWidth(Coord w)
		{
			if(w <= 0) 
				w = 1;
			if(w > MaxPenWidth) w = MaxPenWidth;
			_penWidth = w;
		}

		template<class Display>
		void Painter<Display>::FillRect(Coord left, Coord top, Coord width, Coord height)
		{
			Fill fill(width, height);
			DrawBitmap(fill, left, top);
		}

		template<class Display>
		void Painter<Display>::DrawLine(Coord x1, Coord y1, Coord x2, Coord y2)
		{
			Coord deltaX = x2 - x1;
			Coord deltaY = y2 - y1;
			Coord signX = 1;
			Coord signY = 1;
			Coord dx = 0, dy = 1;
			if(deltaX < 0)
			{
				signX = -1;
				deltaX = -deltaX;
			}

			if(deltaY < 0)
			{
				signY = -1;
				deltaY = -deltaY;
			}

			Coord error = deltaX - deltaY;
			Coord length = max(deltaX, deltaY);

			if(deltaX < deltaY)
			{
				dx = 1;
				dy = 0;
			}

			for (;length; length--)
			{
				for(Coord i = -_penWidth/2; i<(_penWidth+1)/2; i++)
					Display::PutPixel(x1+dx*i, y1+dy*i, _color);
					 
				Coord error2 = error * 2;
	 
				if(error2 > -deltaY)
				{
					error -= deltaY;
					x1 += signX;
				}
	 
				if(error2 < deltaX)
				{
					error += deltaX;
					y1 += signY;
				}
			}
		}

		template<class Display>
		template<class Bitmap>
		void Painter<Display>::DrawBitmap(Bitmap &pic, Coord x, Coord y)
		{
			Display::DrawBitmap(pic, x, y);
		}

		template<class Display>
		template<class Font>
		void Painter<Display>::DrawText(const char *str, Coord x, Coord y, Font &font)
		{
			char c;
			while((c = *str++))
			{
				typename Font::CharType pic = font.GetChar(c);
				Display::DrawBitmap(pic, x, y);
				x += pic.Width();
			}
		}
		
		template<class Display>
		void Painter<Display>::DrawRect(Coord left, Coord top, Coord width, Coord height)
		{
			DrawLine(left, top, left + width, top);
			DrawLine(left, top, left, top + height);
			DrawLine(left + width, top, left + width, top + height);
			DrawLine(left + width, top + height, left, top + height);
		}

		template<class Display>
		void Painter<Display>::CircleHelper(Coord x, Coord y, Coord dx, Coord dy)
		{
			Display::PutPixel(x+dx, y+dy, _color);
			if(dx)
				Display::PutPixel(x-dx, y+dy, _color);
			if(dy)
				Display::PutPixel(x+dx, y-dy, _color);
			if(dx && dy)
				Display::PutPixel(x-dx, y-dy, _color);
		}

		template<class Display>
		void Painter<Display>::DrawCircle(Coord x, Coord y, Coord r)
		{  
			r += _penWidth / 2;
			Coord rInt = max(r - _penWidth, 0);
			Coord dx = 0, dy = r, old_dy = dy, d = 3 - 2*r;
			while (dx <= dy)
			{	
				if(_penWidth == 1)
				{
					CircleHelper(x, y, dx, dy);
					if(dx != dy)
						CircleHelper(x, y, dy, dx);
				}else
				{
					Coord dxLim = sqr(rInt) - sqr(dy);
					Coord dyLim = sqr(rInt) - sqr(dx);
					if(old_dy != dy && dx != dy)
						for(int i=0; sqr(dx - i) > dxLim && dx >= i; i++)
						{
							CircleHelper(x, y, dx-i, dy);
						}
					for(int i=0; sqr(dy - i) > dyLim && dy >= i; i++)
					{
						CircleHelper(x, y, dy-i, dx);
					}
				}
				old_dy = dy;
				if (d < 0)
					d += 4*dx + 6;
				else 
					d += 4*(dx-(dy--)) + 10;
				++dx;
			}
		}
	}
}
#endif
